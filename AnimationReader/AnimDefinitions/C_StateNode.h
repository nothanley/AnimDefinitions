// WIP Reads Nodes and properties within adef binary stream. 
// TODO: All streams are parsed, next cleanup the code and collect everything in proper objects.

#include "C_DefStream.h"
#include "C_AnimNodes.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma once
using namespace StateNode;

class C_StateNode
{

private:
	std::istream* fs;

	std::vector<KeyValueProp> ReadKeyValueProperty(bool isNodeValue);

	Node ReadNodeType1();

	Node ReadNodeType2();

	Node ProcessNode();

	std::vector<Node> ProcessDTTs();

	void ReadCandStream(uint32_t candidateEncoding);

	std::vector<SyncNode> ProcessSyncNode();

	std::vector<std::string> ProcessDescriptor();

	void ReadEventArguments();

	void ReadEventTriggers();

	void ProcessSMEvents();

	void ReadFRMSStream();

	void ProcessMembers();

	void ProcessSelectors();

public:

	C_StateNode(std::istream* binaryStream) {
		this->fs = binaryStream;
	}

	void InitializeStateNode(std::vector<Node>& nodes);
};
