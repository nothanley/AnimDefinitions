#include "C_StateNode.h"


void C_StateNode::InitializeStateNode(std::vector<Node>& nodes)
{
	// Reads all state nodes in adef stream. 
	uint32_t stateNodeSig = ReadUInt32(*fs);
	uint32_t totalStateNodes = ReadUInt32(*fs);

	/* Iterate through all State Nodes. Assess stream format*/
	for (int i = 0; i < totalStateNodes;) {
		Node stateNode;
		uint32_t streamType = ReadUInt32(*fs);

		while (ntohl(streamType) != GRP_) {
			switch (ntohl(streamType)) {
				case(KV__):
					stateNode.keyValueProperties.push_back( ReadKeyValueProperty(false) );
					break;
				case(SYNC):
					stateNode.syncNodes.push_back( ProcessSyncNode() );
					i++;
					break;
				case(DTT_):
					stateNode.transNodes.push_back( ProcessDTTs() );
					break;
				case(TOVR):
					stateNode.ovlyNodes.push_back( ProcessDTTs() );
					break;
				case(NODE):
					stateNode.childNodes.push_back( ProcessNode() );
					break;
				case(DESC):
					stateNode.descriptors = ProcessDescriptor();
					break;
				case(EVNT):
					ProcessSMEvents();
					break;
				default:
					std::cout << fs->tellg();
					std::cout << "\nUnknown Node Stream.";
					break;
			}
			streamType = ReadUInt32(*fs);
		}
		nodes.push_back(stateNode);
	}

	if (totalStateNodes == 0) { ReadUInt32(*fs); }
	uint32_t totalGroupNodes = ReadUInt32(*fs);

	// todo: collect node var
	/* Iterate through all Group Nodes. Assess stream formats*/
	for (int i = 0; i < totalGroupNodes; i++) {
		std::string grpString = ReadString(*fs, ReadUInt32(*fs));

		uint32_t streamType = ReadUInt32(*fs);
		ProcessDescriptor();

		streamType = ReadUInt32(*fs);
		ProcessMembers();

		streamType = ReadUInt32(*fs);
		ProcessSelectors();
	}
}

Node C_StateNode::ReadNodeType1()
{
	Node animNode{ NODE };
	bool nodeFlag = ReadBool(*fs);
	uint32_t streamSig = ReadUInt32(*fs);
	animNode.keyValueProperties.push_back( ReadKeyValueProperty(true) );
	return animNode;
}

Node C_StateNode::ReadNodeType2()
{
	Node animNode{ NODE };
	int32_t nodeFlag = ReadSInt32(*fs);
	uint32_t streamSig = ReadUInt32(*fs);
	animNode.keyValueProperties.push_back(ReadKeyValueProperty(true));

	uint64_t unkVal64 = ReadUInt64(*fs);
	float unkFloat = ReadFloat(*fs);

	//Process 'BARG' stream
	uint32_t bargSig = ReadUInt32(*fs);
	uint32_t numBargs = ReadUInt32(*fs);

	for (int i = 0; i < numBargs; i++) {
		uint64_t unkVal64 = ReadUInt64(*fs);
		float unkFloat = ReadFloat(*fs);
		animNode.childNodes.push_back( ProcessNode() );
	}
	return animNode;
}

std::vector<Node> C_StateNode::ProcessDTTs()
{
	std::vector<Node> dttArray;
	uint32_t numDTTs = ReadUInt32(*fs);

	for (int i = 0; i < numDTTs; i++) {
		Node dttNode{ DTT_ };
		uint32_t blockType = ReadUInt32(*fs);
		dttNode.keyValueProperties.push_back(ReadKeyValueProperty(false));
	}
	return dttArray;
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
	uint32_t numSels = ReadUInt32(*fs);
	uint32_t selType;

	for (int i = 0; i < numSels; i++) {
		selType = ReadUInt32(*fs);
		ReadFRMSStream();
		ReadCandStream(selType);
	}
}

Node C_StateNode::ProcessNode()
{
	Node parentNode{ NODE };
	uint32_t nodeType = ReadUInt32(*fs);

	switch (nodeType) {
	case(0x2):
		parentNode.childNodes.push_back(ReadNodeType2());
		break;
	case(0x1):
		parentNode.childNodes.push_back(ReadNodeType1());
		break;
	default:
		std::cout << "\nInvalid stream type.";
		break;
	}
	return parentNode;
}

std::vector<KeyValueProp> C_StateNode::ReadKeyValueProperty(bool isNodeValue)
{
	std::vector<KeyValueProp> kvProps;
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
