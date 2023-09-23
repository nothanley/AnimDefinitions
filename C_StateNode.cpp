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
		ProcessSMEvents(); // todo: collect this

		animDef.stateNodes.push_back(stateNode);
	}

	/* Iterate through all Group Nodes. Assess stream formats*/
	uint32_t GRPMagic = ReadUInt32(*fs);
	uint32_t numGroups = ReadUInt32(*fs);

	for (int i = 0; i < numGroups; i++) {
		GroupNode group;
		group.chars = ReadString(*fs, ReadUInt32(*fs));
		group.descriptors = ProcessDescriptor();
		ProcessMembers();  // todo: collect this
		ProcessSelectors();  // todo: collect this

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

void C_StateNode::ReadCandStream(uint32_t candidateEncoding)
{
	uint32_t streamSig = ReadUInt32(*fs);
	uint32_t numCands = ReadUInt32(*fs);
	for (int i = 0; i < numCands; i++) {
		uint64_t unkVal1 = ReadUInt64(*fs);
		bool flag = ReadBool(*fs);
	}

	switch (ntohl(candidateEncoding)) {
		case(0xEB7AD409):
			for (int i = 0; i < numCands; i++) {
				ReadFloat(*fs);
				ReadFloat(*fs);
			}
			ReadUInt32(*fs);
			break;
		case(0xF342C816):
			break;
		case(0x76A50FDC):
			ReadUInt64(*fs);
			ReadBool(*fs);
			break;
		case(0x4BFF8647):
			for (int i = 0; i < numCands; i++) {
				ReadFloat(*fs);
			}
			ReadBool(*fs);
			break;
		case(0x2F05210F):
			for (int i = 0; i < numCands; i++) {
				ReadUShort(*fs);
				ReadFloat(*fs);
				ReadFloat(*fs);
			}
			ReadUInt64(*fs);
			ReadBool(*fs);
			break;
		case(0x1A8AECAF):
			for (int i = 0; i < numCands; i++) {
				ReadUShort(*fs);
			}
			ReadUInt64(*fs);
			ReadBool(*fs);
			break;
		default:
			std::cout << fs->tellg();
			std::cout << "\nCould not read candidate node.";
			break;
	}
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

void C_StateNode::ReadEventArguments()
{
	ReadUInt32(*fs); //  0xARGS
	uint32_t numArgs = ReadUInt32(*fs);
	for (int i = 0; i < numArgs; i++) {
		uint16_t argIdx = ReadUShort(*fs);
	}
}

void C_StateNode::ReadEventTriggers()
{
	ReadUInt32(*fs); // 0xTRIGS
	uint32_t numTriggers = ReadUInt32(*fs);
	for (int i = 0; i < numTriggers; i++) {
		uint64_t unkVal1 = ReadUInt64(*fs);
		float unkFloat1 = ReadFloat(*fs);
	}
}

void C_StateNode::ProcessSMEvents()
{
	std::vector<EventNode> events;
	uint32_t streamSig = ReadUInt32(*fs);
	uint32_t numEvents = ReadUInt32(*fs);

	for (int i = 0; i < numEvents; i++) {
		uint64_t unkVal1 = ReadUInt64(*fs);
		uint32_t unkVal2 = ReadUInt32(*fs);
		bool flag = ReadBool(*fs);

		if (!flag) {
			uint32_t unkVal3 = ReadUInt32(*fs);
			uint64_t unkVal4 = ReadUInt64(*fs);
		}

		ReadEventTriggers();
		ReadEventArguments();
	}
}

void C_StateNode::ReadFRMSStream()
{
	ReadUInt32(*fs);
	uint32_t numFrames = ReadUInt32(*fs);

	for (int i = 0; i < numFrames; i++) {
		uint64_t unkVal1 = ReadUInt64(*fs);
		bool flag = ReadBool(*fs);
	}
}

void C_StateNode::ProcessMembers()
{
	uint32_t streamSig = ReadUInt32(*fs);
	uint32_t numMembs = ReadUInt32(*fs);
	uint32_t selType;

	for (int i = 0; i < numMembs; i++)
		uint64_t unkVal1 = ReadUInt64(*fs);

	selType = ReadUInt32(*fs);

	ReadFRMSStream();
	ReadCandStream(selType);
}

void C_StateNode::ProcessSelectors()
{
	uint32_t streamSig = ReadUInt32(*fs);
	uint32_t numSels = ReadUInt32(*fs);
	uint32_t selType;

	for (int i = 0; i < numSels; i++) {
		selType = ReadUInt32(*fs);
		ReadFRMSStream();
		ReadCandStream(selType);
	}
}

Node C_StateNode::ProcessNode(bool isChild)
{
	Node parentNode{ NODE };
	if (!isChild) { uint32_t streamSig = ReadUInt32(*fs); }
	uint32_t nodeType = ReadUInt32(*fs);

	switch (nodeType) {
	case(0x2):
		parentNode.childNodes.push_back(ReadNodeType2());
		break;
	case(0x1):
		parentNode.childNodes.push_back(ReadNodeType1());
		break;
	default:
		std::cout << fs->tellg();
		std::cout << "\nInvalid stream type.";
		break;
	}
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
