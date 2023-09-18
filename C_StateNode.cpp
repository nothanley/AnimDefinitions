#include "C_StateNode.h"


void C_StateNode::InitializeStateNode(std::vector<AnimDef>& nodes)
{
	// Reads all state nodes in adef stream. 
	uint32_t stateNodeSig = ReadUInt32(*fs);
	uint32_t totalStateNodes = ReadUInt32(*fs);

	/* Iterate through all State Nodes. Assess stream format*/
	for (int i = 0; i < totalStateNodes;) {
		AnimDef adefNode;
		uint32_t streamType = ReadUInt32(*fs);

		while (ntohl(streamType) != GRP_) {
			switch (ntohl(streamType)) {
				case(KV__):
					adefNode.kvData = ReadKeyValueProperty(false);
					break;
				case(SYNC):
					ProcessSyncNode();
					i++;
					break;
				case(DTT_):
					ProcessDTTs();
					break;
				case(TOVR):
					ProcessDTTs();
					break;
				case(NODE):
					adefNode.nodes.push_back( ProcessNode() );
					break;
				case(DESC):
					adefNode.descriptors = ProcessDescriptor();
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
		nodes.push_back(adefNode);
	}

	if (totalStateNodes == 0) { ReadUInt32(*fs); }
	uint32_t totalGroupNodes = ReadUInt32(*fs);

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

AnimDef C_StateNode::ReadNodeType1()
{
	AnimDef animNode{NODE};
	bool nodeFlag = ReadBool(*fs);
	uint32_t streamSig = ReadUInt32(*fs);
	animNode.kvData = ReadKeyValueProperty(true);
	return animNode;
}

AnimDef C_StateNode::ReadNodeType2()
{
	AnimDef animNode{ NODE };
	int32_t nodeFlag = ReadSInt32(*fs);
	uint32_t streamSig = ReadUInt32(*fs);
	animNode.kvData = ReadKeyValueProperty(true);

	uint64_t unkVal64 = ReadUInt64(*fs);
	float unkFloat = ReadFloat(*fs);

	//Process 'BARG' stream
	uint32_t bargSig = ReadUInt32(*fs);
	uint32_t numBargs = ReadUInt32(*fs);

	for (int i = 0; i < numBargs; i++) {
		uint64_t unkVal64 = ReadUInt64(*fs);
		float unkFloat = ReadFloat(*fs);
		animNode.nodes.push_back( ProcessNode() );
	}
	return animNode;
}

std::vector<AnimDef> C_StateNode::ProcessDTTs()
{
	std::vector<AnimDef> dttArray;
	uint32_t numDTTs = ReadUInt32(*fs);

	for (int i = 0; i < numDTTs; i++) {
		AnimDef dttNode{ DTT_ };
		uint32_t blockType = ReadUInt32(*fs);
		dttNode.kvData = ReadKeyValueProperty(false);
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

std::vector<kvProperty> C_StateNode::ReadKeyValueProperty(bool isNodeValue)
{
	std::vector<kvProperty> kvProps;
	uint32_t numKVs = ReadUInt32(*fs);

	for (int i = 0; i < numKVs; i++) {
		kvProperty kvProp;
		kvProp.streamSig = ReadUInt32(*fs);

		switch (ntohl(kvProp.streamSig)) {
			case (0x46C0C6BF):
				kvProp.val8 = ReadBool(*fs);
				break;
			case (0xA8FA930E):
				kvProp.val8 = ReadBool(*fs);
				break;
			case (0x4D245F2B):
				kvProp.val8 = ReadBool(*fs);
				break;
			case (0x6782648B):
				kvProp.val8 = ReadBool(*fs);
				break;
			case (0xDF4B0B8E):
				if (!isNodeValue) { kvProp.chars = ReadString(*fs, ReadUInt32(*fs)); }
				else { kvProp.val64 = ReadUInt64(*fs); }
				break;
			case (0x60C09DC4):
				kvProp.val8 = ReadBool(*fs);
				break;
			case (0xA6D0A4EB):
				kvProp.val8 = ReadBool(*fs);
				break;
			case (0x5D651EEF):
				kvProp.val8 = ReadBool(*fs);
				break;
			case (0x4425DADA):
				kvProp.val32 = ReadUInt32(*fs);
				break;
			case (0x45991D01):
				kvProp.chars = ReadString(*fs, ReadUInt32(*fs));
				break;
			case (0x48DE1308):
				kvProp.val8 = ReadBool(*fs);
				break;
			case (0x73EDDE33):
				kvProp.val64 = ReadUInt64(*fs);
				break;
			case (0x0C3F823A):
				kvProp.val8 = ReadBool(*fs);
				break;
			case (0x654680ED):
				kvProp.val32 = ReadUInt32(*fs);
				break;
			case (0xC248BCF4 ):
				kvProp.val8 = ReadBool(*fs);
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
				kvProp.val8 = ReadBool(*fs);
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
