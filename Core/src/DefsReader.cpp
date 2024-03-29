#include "StateNode.h"
#include "Compression/zcompress.h"
#include <sstream>
#include "DefsReader.h"
#include "BinaryIO.h"
#include <winsock.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#pragma once

using namespace BinaryIO;

ADefHandler::ADefHandler(const char* filePath) {
	openFile(filePath);
}

void 
ADefHandler::openFile(const char* FilePath) {
	// verifies if file is readable
	m_cFilePath = FilePath;
	ifstream inFile(m_cFilePath);

	if (inFile.good())
		fileBuffer->open(m_cFilePath, ios::in | ios::binary);
	else
		throw("Cannot Open File.");

	// store filepath&verify type/version
	this->m_iFileSize = getFileSize(inFile);
	fs = new std::istream(fileBuffer);
	ValidateADEFs();
	if (!isValidFile) { return; }

	// reset-seek
	fs->seekg(0);
	fileBuffer->pubseekpos(ios_base::beg);
	LoadAnimDefData();

	printf("\nParsing complete.");

	// clean memory
	fs->clear();
	fileBuffer->close();
	if (zlibBuffer != nullptr) { zlibBuffer->clear(); delete zlibBuffer; }
	delete fs;
}


void 
ADefHandler::LoadArguments(int size) {
	/* Reads all binary arguments. Stores in argument array */
	for (int i = 0; i < size; i++) {
		DefArg arguement;
		arguement.identifier = ReadUInt64(*fs);
		arguement.type = ReadByte(*fs);

		switch (arguement.type) {
		case 0x5:
			arguement.value = ReadUInt64(*fs); // list or qword
			break;
		case 0x4:
			arguement.charSize = ReadUInt32(*fs);
			arguement.name = ReadString(*fs, arguement.charSize); // string
			break;
		case 0x3:
			arguement.value = ReadUInt32(*fs); // float?
			break;
		case 0x2:
			arguement.value = ReadUInt32(*fs); // value
			break;
		case 0x1:
			arguement.value = ReadByte(*fs); // bool
			break;
		default:
			break;
		}
		m_Arguments.push_back(arguement);
	}

}

void 
ADefHandler::LoadCommands(int size) {
	/* Reads all 64-bit binary commands. Does not decode values, just stores in array */
	std::vector<uint64_t> cmds;

	for (int i = 0; i < size; i++) {

		uint64_t value = ReadUInt64(*fs);
		cmds.push_back(value);
	}
}

void 
ADefHandler::LoadStateMirror(int size) {

	/* Reads Event Mirror Defs */
	/* ( collects all binary values but lacks interpretation logic )*/
	std::vector<uint64_t> mirrorCmdMap;
	std::vector<uint16_t> mirrorArgsMap;
	std::vector<uint64_t> mirrorDescMap;
	std::vector<uint32_t> mirrorBlendVarMap;

	uint32_t commandsCount = ReadUInt32(*fs);
	for (int i = 0; i < commandsCount; i++) {
		/* In game logic, these are compared with project strings
		until a match is made and outputting its index */
		uint64_t cmdHash_0 = ReadUInt64(*fs);
		uint64_t cmdHash_1 = ReadUInt64(*fs);
		mirrorCmdMap.push_back(cmdHash_0);
		mirrorCmdMap.push_back(cmdHash_1);
	}

	uint32_t argsCount = ReadUInt32(*fs);
	for (int i = 0; i < argsCount; i++) {
		uint16_t argPrm_0 = ReadUShort(*fs);
		uint16_t argPrm_1 = ReadUShort(*fs);
		mirrorArgsMap.push_back(argPrm_0);
		mirrorArgsMap.push_back(argPrm_1);
	}

	uint32_t descriptorCount = ReadUInt32(*fs);
	for (int i = 0; i < descriptorCount; i++) {
		uint64_t descHash_0 = ReadUInt64(*fs);
		uint64_t descHash_1 = ReadUInt64(*fs);
		mirrorDescMap.push_back(descHash_0);
		mirrorDescMap.push_back(descHash_1);
	}

	uint32_t blendVarCount = ReadUInt32(*fs);
	for (int i = 0; i < blendVarCount; i++) {
		uint32_t blvr = ReadUInt32(*fs);
		ReadByte(*fs);
		mirrorBlendVarMap.push_back(blvr);
	}
}

std::vector<char> 
ADefHandler::CacheBinaryData(std::istream* fs, uint64_t filePos) {
	std::vector<char> binaryData; // Init vector
	fs->seekg(0);
	uint64_t bytesToRead = filePos;
	binaryData.resize(bytesToRead); // Read Binary
	fs->read(binaryData.data(), bytesToRead);
	fs->seekg(filePos); // Return place
	return binaryData;
}

void 
ADefHandler::LoadAnimDef(int size) {
	uint64_t filePos = fs->tellg();
	this->m_metaStream = CacheBinaryData(fs, filePos - 0x8);
	fs->seekg(filePos);
	for (int i = 0; i < size; i++) {
		C_StateNode newStateNode(fs);
		newStateNode.InitializeDefinitions(this->m_Definitions);
	}
}

/* Interprets all binary streams within animdef */
void 
ADefHandler::LoadAnimDefData() {
	uint32_t fileMagic;
	uint32_t versionFlag;
	uint32_t binType;
	uint32_t binCount;
	ProjectDefs cProjDefs(fs);

	fileMagic = ReadUInt32(*fs);
	versionFlag = ReadUInt32(*fs);

	while (uint32_t(fs->tellg()) <= this->m_iFileSize) {
		binType = ReadUInt32(*fs);
		binCount = ReadUInt32(*fs);

		switch (binType) {
		case (PROJ):
			cProjDefs.ParseProjectXML(binCount);
			break;
		case (ARGS):
			LoadArguments(binCount);
			break;
		case (CMDS):
			LoadCommands(binCount);
			break;
		case (STMR):
			LoadStateMirror(binCount);
			break;
		case (ADEF):
			LoadAnimDef(binCount);
			break;
		default:
			/* Covers general animdef streams */
			cProjDefs.ParseProjectXML(binCount);
			break;
		}
	}
}

void 
ADefHandler::decompressFile() {
	uint32_t dataSize;
	std::ifstream cmpStream;
	std::stringstream buffer;
	fs->seekg(0);   // Clear all current buffers
	buffer << fs->rdbuf();
	buffer.seekg(0);

	cmpStream.set_rdbuf(buffer.rdbuf());    // Decompress ifstream
	byte* data = ADef::DecompressMemoryStream(&cmpStream, &dataSize);
	zlibBuffer = new std::stringstream;
	zlibBuffer->write((char*)data, dataSize);

	//clear remaining streams and set pointers
	fileBuffer->close(); fs->clear();
	if (data != nullptr) { this->isValidFile = true; }
	this->m_iFileSize = dataSize;
	fs->set_rdbuf(zlibBuffer->rdbuf());
}

void 
ADefHandler::ValidateADEFs() {
	uint32_t magicSig;
	//seeks magic and validates
	fileBuffer->pubseekpos(ios_base::beg);
	magicSig = ReadUInt32(*fs);
	magicSig = ntohl(magicSig);

	//validate type and version
	if (magicSig != TYPE && magicSig != VER_) {
		std::cout << ("File is invalid adefs container.");
		return;
	}

	if (magicSig == VER_) { decompressFile(); } // Decompress ZLIB
	isValidFile = true;
}

uint64_t 
ADefHandler::getFileSize(std::ifstream& fileStream)
{
	std::streampos currentPosition = fileStream.tellg();
	fileStream.seekg(0, std::ios::end);
	std::streampos fileSize = fileStream.tellg();
	fileStream.seekg(currentPosition, std::ios::beg);
	return uint32_t(fileSize);
}



