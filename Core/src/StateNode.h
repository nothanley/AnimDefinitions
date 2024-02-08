// Reads Nodes and properties within 'ADEF' binary stream and populates C++ Object with keys and values 

#include "Definition.h"
#include "AnimNode.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma once

using namespace StateNode;

class C_StateNode
{

public:

	C_StateNode(std::istream* binaryStream) {
		this->fs = binaryStream;
	}

	void InitializeDefinitions(std::vector<StateNode::Definition>& nodes);

private:
	std::istream* fs;

    void CollectStateNodes(StateNode::Definition* animDef);

    void CollectGroupNodes(StateNode::Definition* animDef);

	void ProcessBargNode(Node* parentNode);

	void ReadFRMSStream(StateNode::MemberNode* member);

	void ReadCandStream(StateNode::MemberNode* member);

	void getSyncNodes(StateNode::Node* state);

	std::vector<KeyValueProp> ReadKeyValueProperty(bool isNodeValue=true);

	Node ReadNodeType1();

	Node ReadNodeType2();

	Node ProcessNode(bool isChild=false);

    std::vector<Node> ProcessTransNode();

	std::vector<SyncNode> ProcessSyncNode(StateNode::Node* node);

	std::vector<std::string> ProcessDescriptor();

	std::vector<EventArgument>  ReadEventArguments();

	std::vector<EventTrigger>  ReadEventTriggers();

	std::vector<EventNode> ProcessSMEvents();

	MemberNode ProcessMembers();

	SelectorNode ProcessSelectors();


};

