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

    void CollectStateNodes(StateNode::Definition* animDef);

    void CollectGroupNodes(StateNode::Definition* animDef);

	std::vector<KeyValueProp> ReadKeyValueProperty(bool isNodeValue=true);

	Node ReadNodeType1();

	Node ReadNodeType2();

	Node ProcessNode(bool isChild=false);

    std::vector<Node> ProcessTransNode();

    void ProcessBargNode(Node *parentNode);

    std::vector<SyncNode> ProcessSyncNode(StateNode::Node* node);

    std::vector<std::string> ProcessDescriptor();

    std::vector<EventArgument>  ReadEventArguments();

    std::vector<EventTrigger>  ReadEventTriggers();

    std::vector<EventNode> ProcessSMEvents();

	void ReadFRMSStream(StateNode::MemberNode* member);

	void ReadCandStream(StateNode::MemberNode* member );

	MemberNode ProcessMembers();

	SelectorNode ProcessSelectors();

    void getSyncNodes(StateNode::Node* state);

public:

	C_StateNode(std::istream* binaryStream) {
		this->fs = binaryStream;
	}

	void InitializeDefinitions(std::vector<StateNode::Definition>& nodes);
};

