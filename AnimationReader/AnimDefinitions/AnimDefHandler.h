/* Initializes a C++ object utilizing animation ProjectBin files */
#include "C_StateNode.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma once

using namespace DefNode;
using namespace StateNode;
using namespace std;
using namespace BinaryIO;

class ADefHandler
{

	std::filebuf* fileBuffer = new std::filebuf();
	uint32_t m_iFileSize;
	std::istream* fs;

public:
    string m_cFilePath;
    std::vector<char> m_metaStream; // holds all non 'adef' streams
    std::vector<StateNode::Definition> m_Definitions;
    std::vector<DefArg> m_Arguments;

	enum {
		TYPE = 0x65707974,
		PROJ = 0x70726f6a,
		ARGS = 0x61726773,
		OVOR = 0x726F766F,
		OVLY = 0x6F766C79,
		VARI = 0x76617269,
		EVNT = 0x65766E74,
		ENUM = 0x656E756D,
		DESC = 0x64657363,
		CMDS = 0x636D6473,
		STMR = 0x73746D72,
		ADEF = 0x61646566
	};

    ADefHandler(int type=TYPE){}
	ADefHandler(const char* filePath) {
		openFile(filePath);
	}

	void openFile(const char* FilePath) {

		// verifies if file is readable
		ifstream inFile(FilePath);

		if (inFile.good())
			fileBuffer->open(FilePath, ios::in | ios::binary);
		else
			throw("Cannot Open File.");

		// store size
		this->m_iFileSize = getFileSize(inFile);

		// store filepath&verify type/version
		m_cFilePath = FilePath;
		fs = new std::istream(fileBuffer);
		ValidateADEFs();

		// reset-seek
		fileBuffer->pubseekpos(ios_base::beg);

		// Gather Basic metadata
		LoadAnimDefData();
		fileBuffer->close();
	}

private:

	void LoadArguments(int size) {
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
	
	void LoadCommands(int size) {
		/* Reads all 64-bit binary commands. Does not decode values, just stores in array */
		std::vector<uint64_t> cmds;

		for (int i = 0; i < size; i++) {

			uint64_t value = ReadUInt64(*fs);
			cmds.push_back(value);
		}
	}

	void LoadStateMirror(int size) {

		/* Reads Event Mirror Defs */
		/* ( collects all binary values but lacks interpretation logic )*/
		std::vector<uint64_t> mirrorCmdMap;
		std::vector<uint16_t> mirrorArgsMap;
		std::vector<uint64_t> mirrorDescMap;
		std::vector<uint32_t> mirrorBlendVarMap;

		DWORD commandsCount = ReadUInt32(*fs);
		for (int i = 0; i < commandsCount; i++) {
			/* In game logic, these are compared with project strings
			until a match is made and outputting its index */
			uint64_t cmdHash_0 = ReadUInt64(*fs);
			uint64_t cmdHash_1 = ReadUInt64(*fs);
			mirrorCmdMap.push_back(cmdHash_0);
			mirrorCmdMap.push_back(cmdHash_1);
		}

		DWORD argsCount = ReadUInt32(*fs);
		for (int i = 0; i < argsCount; i++) {
			uint16_t argPrm_0 = ReadUShort(*fs);
			uint16_t argPrm_1 = ReadUShort(*fs);
			mirrorArgsMap.push_back(argPrm_0);
			mirrorArgsMap.push_back(argPrm_1);
		}

		DWORD descriptorCount = ReadUInt32(*fs);
		for (int i = 0; i < descriptorCount; i++) {
			uint64_t descHash_0 = ReadUInt64(*fs);
			uint64_t descHash_1 = ReadUInt64(*fs);
			mirrorDescMap.push_back(descHash_0);
			mirrorDescMap.push_back(descHash_1);
		}

		DWORD blendVarCount = ReadUInt32(*fs);
		for (int i = 0; i < blendVarCount; i++) {
			uint32_t blvr = ReadUInt32(*fs);
			ReadByte(*fs);
			mirrorBlendVarMap.push_back(blvr);
		}
	}

    std::vector<char> CacheBinaryData( std::istream* fs , uint64_t filePos ){
        std::vector<char> binaryData; // Init vector
        fs->seekg(0);
        uint64_t bytesToRead = filePos;
        binaryData.resize(bytesToRead); // Read Binary
        fs->read(binaryData.data(), bytesToRead);
        fs->seekg(filePos); // Return place
        return binaryData;
    }

	void LoadAnimDef(int size) {
        uint64_t filePos = fs->tellg();
        this->m_metaStream = CacheBinaryData( fs, filePos-0x8 );
        fs->seekg(filePos);
		for (int i = 0; i < size; i++) {
			C_StateNode newStateNode(fs);
            newStateNode.InitializeDefinitions(this->m_Definitions); }
	}

	/* Interprets all binary streams within animdef */
	void LoadAnimDefData() {
		DWORD fileMagic;
		DWORD versionFlag;
		DWORD binType;
		DWORD binCount;
		ProjectDefs cProjDefs(fs);

		fileMagic = ReadUInt32(*fs);
		versionFlag = ReadUInt32(*fs);

        while ( uint32_t(fs->tellg()) <= this->m_iFileSize) {
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

	void ValidateADEFs() {
		DWORD magicSig;

		//seeks magic and validates
		fileBuffer->pubseekpos(ios_base::beg);
		magicSig = ReadUInt32(*fs);

		//validate type and version
		if (ntohl(magicSig) != TYPE) {
			throw("File is invalid ANIMs container.");
		}

	}

	uint32_t getFileSize(std::ifstream& fileStream)
	{
		std::streampos currentPosition = fileStream.tellg();
		fileStream.seekg(0, std::ios::end);
		std::streampos fileSize = fileStream.tellg();
		fileStream.seekg(currentPosition, std::ios::beg);
		return uint32_t(fileSize);
	}


};
