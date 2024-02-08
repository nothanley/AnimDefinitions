/* Initializes a C++ object for animation definition binaries,
	reads through all streams and stores child PROJ and ADEF objects */

#include <string>
#include <vector>
#include <fstream>
#include "StateNode.h"
#pragma once
using namespace std;

class ADefHandler
{

public:
    string m_cFilePath;
    std::vector<char> m_metaStream; // holds all non 'adef' streams
    std::vector<StateNode::Definition> m_Definitions;
    std::vector<DefArg> m_Arguments;
    std::vector<std::string> m_aEventTitles;
    std::vector<uint64_t> m_aEventHashes;
    bool isValidFile = false;

	enum {
        VER_ = 0x5F726576,
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

	ADefHandler(int type = TYPE) {};
	ADefHandler(const char* filePath);
	void openFile(const char* FilePath);

private:
    std::filebuf* m_bFileStream  = new std::filebuf();
	uint32_t m_iFileSize;
	std::istream* fs;
    std::stringstream* m_bZlibStream = nullptr;

    void GetEventIDs();
	void LoadArguments(int size);
	void LoadCommands(int size);
	void LoadStateMirror(int size);
    std::vector<char> CacheBinaryData( std::istream* fs , uint64_t filePos );
	void LoadAnimDef(int size);
	void LoadAnimDefData(); /* Interprets all binary streams within animdef */
    void decompressFile();
	void ValidateADEFs();
	uint64_t getFileSize(std::ifstream& fileStream);

};
