#include "C_StateNode.h"


void C_StateNode::InitializeDefinitions(std::vector<StateNode::Definition>& aDefCollection)
{
	// Initializes a Definition obj containing State & Group Nodes
	StateNode::Definition animDef;

	/* Iterate through all State Nodes. Assess stream format*/
	uint32_t STATMagic = ReadUInt32(*fs);
	uint32_t numStates = ReadUInt32(*fs);

	for (int i = 0; i < numStates; i++) {
		Node stateNode{ STAT };
		stateNode.keyValueProperties.push_back( ReadKeyValueProperty(false) );
		stateNode.syncNodes = ProcessSyncNode();
		stateNode.transNodes.push_back(ProcessTransNode() );
		stateNode.ovlyNodes.push_back(ProcessTransNode() );
		stateNode.childNodes.push_back( ProcessNode() );
		stateNode.descriptors = ProcessDescriptor();
        stateNode.events = ProcessSMEvents(); // todo: collect this

		animDef.stateNodes.push_back(stateNode);
	}

	/* Iterate through all Group Nodes. Assess stream formats*/
	uint32_t GRPMagic = ReadUInt32(*fs);
	uint32_t numGroups = ReadUInt32(*fs);

	for (int i = 0; i < numGroups; i++) {
		GroupNode group;
		group.chars = ReadString(*fs, ReadUInt32(*fs));
		group.descriptors = ProcessDescriptor();
		group.members = ProcessMembers();
		group.selectors = ProcessSelectors();
		animDef.groupNodes.push_back(group);
	}

	aDefCollection.push_back(animDef);
}

Node C_StateNode::ReadNodeType1()
{
	Node animNode{ NODE };
	bool nodeFlag = ReadBool(*fs);
	animNode.keyValueProperties.push_back( ReadKeyValueProperty() );
	return animNode;
}

Node C_StateNode::ReadNodeType2()
{
	Node animNode{ NODE };
	int32_t nodeFlag = ReadSInt32(*fs);
	animNode.keyValueProperties.push_back(ReadKeyValueProperty());
	uint64_t unkVal64 = ReadUInt64(*fs);
	float unkFloat = ReadFloat(*fs);

	//Process 'BARG' stream
	uint32_t bargSig = ReadUInt32(*fs);
	uint32_t numBargs = ReadUInt32(*fs);
	for (int i = 0; i < numBargs; i++) {
		uint64_t unkVal64 = ReadUInt64(*fs);
		float unkFloat = ReadFloat(*fs);
		animNode.childNodes.push_back( ProcessNode(true) );
	}

	return animNode;
}

Node C_StateNode::ProcessTransNode()
{
	Node animNode;
	uint32_t streamSig = ReadUInt32(*fs);
	uint32_t numNodes = ReadUInt32(*fs);
	animNode.nodeType = ( ntohl(streamSig) == DTT_ ? DTT_ : TOVR);

	for (int i = 0; i < numNodes; i++) {
		animNode.keyValueProperties.push_back(ReadKeyValueProperty());
	}

	return animNode;
}


std::vector<SyncNode> C_StateNode::ProcessSyncNode()
{
	uint32_t streamSig = ReadUInt32(*fs);
	std::vector<SyncNode> syncNodes;
	uint32_t numSyncNodes = ReadUInt32(*fs);

	for (int i = 0; i < numSyncNodes; i++) {
		SyncNode sNode;
		sNode.valueA = ReadUInt64(*fs);
		sNode.valueB = ReadUInt64(*fs);
		sNode.flag = ReadBool(*fs);
        syncNodes.push_back(sNode);
	}

	ReadUInt64(*fs); // 0x0 constant
	return syncNodes;
}

std::vector<std::string> C_StateNode::ProcessDescriptor()
{
	std::vector<std::string> descriptors;
	uint32_t streamSig = ReadUInt32(*fs);
	uint32_t numDescs = ReadUInt32(*fs);

	for (int i = 0; i < numDescs; i++) {
		std::string chars = ReadString(*fs, ReadUInt32(*fs));
		descriptors.push_back(chars);
	}
	return descriptors;
}


std::vector<EventArgument> C_StateNode::ReadEventArguments()
{
    uint32_t streamSig = ReadUInt32(*fs); //  0xARGS
    uint32_t numArgs = ReadUInt32(*fs);
    std::vector<EventArgument> argCollection;

    for (int i = 0; i < numArgs; i++)
        argCollection.push_back(EventArgument{ ReadUShort(*fs) });

    return argCollection;
}

std::vector<EventTrigger> C_StateNode::ReadEventTriggers()
{
    uint32_t streamSig = ReadUInt32(*fs); // 0xTRIGS
    uint32_t numTriggers = ReadUInt32(*fs);
    std::vector<EventTrigger> trigCollection;

    for (int i = 0; i < numTriggers; i++)
        trigCollection.push_back(
            EventTrigger{ ReadUInt64(*fs), ReadFloat(*fs) } );

    return trigCollection;
}

std::vector<EventNode> C_StateNode::ProcessSMEvents()
{
    std::vector<EventNode> events;
    uint32_t streamSig = ReadUInt32(*fs);
    uint32_t numEvents = ReadUInt32(*fs);

    for (int i = 0; i < numEvents; i++) {
        EventNode newEvent{ ReadUInt64(*fs),
            ReadUInt32(*fs), ReadBool(*fs) };

        if (!newEvent.flag) {
            newEvent.value_2 = ReadUInt32(*fs);
            newEvent.value_3 = ReadUInt64(*fs);
        }

        newEvent.triggers = ReadEventTriggers();
        newEvent.arguments = ReadEventArguments();
        events.push_back(newEvent);
    }
    return events;
}

void C_StateNode::ReadFRMSStream(StateNode::MemberNode* member)
{
	uint32_t streamSig = ReadUInt32(*fs);
	uint32_t numFrames = ReadUInt32(*fs);

	for (int i = 0; i < numFrames; i++)
		member->frames.push_back( StateNode::Frame{ ReadUInt64(*fs) ,  ReadBool(*fs) } );
}

void C_StateNode::ReadCandStream(StateNode::MemberNode* member)
{
    uint32_t streamSig = ReadUInt32(*fs);
    uint32_t numCands = ReadUInt32(*fs);

    for (int i = 0; i < numCands; i++) {
		member->candidates.push_back( StateNode::Candidate{
			ReadUInt64(*fs) ,  ReadBool(*fs) });
    }

    switch (ntohl(member->type)) {
        case(0xEB7AD409):
            for (int i = 0; i < numCands; i++) {
				member->candidates[i].fValue_0 = ReadFloat(*fs);
				member->candidates[i].fValue_1 = ReadFloat(*fs);
            }
			member->dValue_0 = ReadUInt32(*fs);
            break;
        case(0xF342C816):
            break;
        case(0x76A50FDC):
			member->lValue_0 = ReadUInt64(*fs);
			member->encodeFlag = ReadBool(*fs);
            break;
        case(0x4BFF8647):
            for (int i = 0; i < numCands; i++) {
				member->candidates[i].fValue_0 = ReadFloat(*fs);
            }
			member->encodeFlag = ReadBool(*fs);
            break;
        case(0x2F05210F):
            for (int i = 0; i < numCands; i++) {
				member->candidates[i].sValue_0 = ReadUShort(*fs);
				member->candidates[i].fValue_0 = ReadFloat(*fs);
				member->candidates[i].fValue_1 = ReadFloat(*fs);
            }
			member->lValue_0 = ReadUInt64(*fs);
			member->encodeFlag = ReadBool(*fs);
            break;
        case(0x1A8AECAF):
            for (int i = 0; i < numCands; i++) {
				member->candidates[i].sValue_0 = ReadUShort(*fs);
            }
			member->lValue_0 = ReadUInt64(*fs);
			member->encodeFlag = ReadBool(*fs);
            break;
        default:
            std::cout << fs->tellg();
            std::cout << "\nCould not read candidate node.";
            break;
    }
}

MemberNode C_StateNode::ProcessMembers()
{
	MemberNode member;
	uint32_t streamSig = ReadUInt32(*fs);
	uint32_t numMembs = ReadUInt32(*fs);

	for (int i = 0; i < numMembs; i++)
		member.values.push_back(ReadUInt64(*fs));

	member.type = ReadUInt32(*fs);
	ReadFRMSStream(&member);
	ReadCandStream(&member);
	return member;
}

SelectorNode C_StateNode::ProcessSelectors()
{
	SelectorNode newSelector;
	uint32_t streamSig = ReadUInt32(*fs);
	uint32_t numMembs = ReadUInt32(*fs);

	for (int i = 0; i < numMembs; i++) {
		MemberNode member;
		member.type = ReadUInt32(*fs);
		ReadFRMSStream(&member);
		ReadCandStream(&member);
		newSelector.members.push_back(member);
	}
	return newSelector;
}

Node C_StateNode::ProcessNode(bool isChild)
{
	Node parentNode{ NODE };
	if (!isChild) { uint32_t streamSig = ReadUInt32(*fs); }
	uint32_t nodeType = ReadUInt32(*fs);

	switch (nodeType) {
	case(0x2):
        parentNode = ReadNodeType2();
		break;
	case(0x1):
        parentNode = ReadNodeType1();
		break;
	default:
		std::cout << fs->tellg();
		std::cout << "\nInvalid stream type.";
        break;  }

	return parentNode;
}

std::vector<KeyValueProp> C_StateNode::ReadKeyValueProperty(bool isNodeValue)
{
	std::vector<KeyValueProp> kvProps;
	uint32_t streamSig = ReadUInt32(*fs);
	uint32_t numKVs = ReadUInt32(*fs);

	for (int i = 0; i < numKVs; i++) {
		KeyValueProp kvProp;
		kvProp.streamKey = ReadUInt32(*fs);

		switch (ntohl(kvProp.streamKey)) {
			case (0x46C0C6BF):
				kvProp.flag = ReadBool(*fs);
				break;
			case (0xA8FA930E):
				kvProp.flag = ReadBool(*fs);
				break;
			case (0x4D245F2B):
				kvProp.flag = ReadBool(*fs);
				break;
			case (0x6782648B):
				kvProp.flag = ReadBool(*fs);
				break;
			case (0xDF4B0B8E):
				if (!isNodeValue) { kvProp.chars = ReadString(*fs, ReadUInt32(*fs)); }
				else { kvProp.val64 = ReadUInt64(*fs); }
				break;
			case (0x60C09DC4):
				kvProp.flag = ReadBool(*fs);
				break;
			case (0xA6D0A4EB):
				kvProp.flag = ReadBool(*fs);
				break;
			case (0x5D651EEF):
				kvProp.flag = ReadBool(*fs);
				break;
			case (0x4425DADA):
				kvProp.val32 = ReadUInt32(*fs);
				break;
			case (0x45991D01):
				kvProp.chars = ReadString(*fs, ReadUInt32(*fs));
				break;
			case (0x48DE1308):
				kvProp.flag = ReadBool(*fs);
				break;
			case (0x73EDDE33):
				kvProp.val64 = ReadUInt64(*fs);
				break;
			case (0x0C3F823A):
				kvProp.flag = ReadBool(*fs);
				break;
			case (0x654680ED):
				kvProp.val32 = ReadUInt32(*fs);
				break;
			case (0xC248BCF4 ):
				kvProp.flag = ReadBool(*fs);
				break;
			case (0x8AE38AFE):
				kvProp.val32 = ReadUInt32(*fs);
				break;
			case (0x8E0B4BDF):
				kvProp.val64 = ReadUInt64(*fs);
				break;
			case (0xBE83F7FD):
				kvProp.chars = ReadString(*fs, ReadUInt32(*fs));
				break;
			case (0x78D548C7):
				kvProp.val32 = ReadUInt32(*fs);
				break;
			case (0xC8B22C04):
				kvProp.valFloat = ReadFloat(*fs);
				break;
			case (0x19981A6F):
				kvProp.valFloat = ReadFloat(*fs);
				break;
			case (0x9E3F108C):
				kvProp.chars = ReadString(*fs, ReadUInt32(*fs));
				break;
			case (0x4C980DCF):
				kvProp.valFloat = ReadFloat(*fs);
				break;
			case (0xE39D3DA5):
				kvProp.valFloat = ReadFloat(*fs);
				break;
			case (0x0A2E3AF4):
				kvProp.flag = ReadBool(*fs);
				break;

			default:
				std::cout << fs->tellg();
				std::cout << "\nInvalid KV value. Cannot KV stream.";
				break;
		}

		kvProps.push_back(kvProp);
	}

	return kvProps;
}