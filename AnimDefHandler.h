/* Initializes a C++ object utilizing animation ProjectBin files */
#include "DefsReader.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma once

using namespace DefNode;
using namespace std;
using namespace BinaryIO;

class ADefHandler
{
	struct DefArg {
		std::string name;
		uint8_t type;
		uint64_t identifier;
		uint32_t charSize;
		uint16_t value;
	};

	std::filebuf* fileBuffer = new std::filebuf();
	string m_cFilePath;
	float m_fileVersion;
	uint32_t m_iProjectType;
	uint32_t m_iFileSize;
	std::istream* fs;

public:

	enum {
		TYPE = 0x65707974,
		PROJ = 0x6A6F7270,
		ARGS = 0x73677261,
		OVOR = 0x726F766F,
		OVLY = 0x6F766C79,
		VARI = 0x76617269,
		EVNT = 0x65766E74,
		ENUM = 0x656E756D,
		DESC = 0x64657363,
		CMDS = 0x636D6473
	};

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

	void LoadArguements(int size) {
		std::vector<DefArg> args;

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
			args.push_back(arguement);
		}

	}

	/* Interprets every stream entry within animdef file object */
	void LoadAnimDefData() {
		DWORD fileMagic;
		DWORD versionFlag;
		DWORD binType;
		DWORD binCount;
		ProjectDefs cProjDefs(fs);

		fileMagic = ReadUInt32(*fs);
		versionFlag = ReadUInt32(*fs);

		while (fs->tellg() <= this->m_iFileSize) {
			binType = ReadUInt32(*fs);
			binCount = ReadUInt32(*fs);

			switch (ntohl(binType)) {
				case (PROJ):
					cProjDefs.ParseProjectXML(binCount);
					break;
				case (ARGS):
					LoadArguements(binCount);
					break;
				case (CMDS):
					LoadArguements(binCount);
					break;
				// TODO: Jun18- Add CMDS and STMR parsers
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
