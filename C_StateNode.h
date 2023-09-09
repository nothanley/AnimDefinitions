#include "C_DefStream.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma once

namespace StateNode {
	
	struct SyncNode {
		uint64_t valueA;
		uint64_t valueB;
		uint8_t flag;
	};

	struct EventNode {
		std::vector<uint64_t> triggers;
		std::vector<uint32_t> arguments;
	};

	struct MemberNode {
		std::vector<uint32_t> frames;
		std::vector<uint64_t> candidates;
	};

	struct GroupNode {
		std::vector<std::string> groupDescriptors;
		std::vector<MemberNode> members;
		std::vector<MemberNode> selectors;
	};

	struct kvProperty {
		uint32_t streamSig;
		std::string chars;
		uint64_t val64;
		uint32_t val32;
		uint16_t val16;
		uint8_t val8;
	};

	struct AnimDef {

		uint32_t streamType; // Differentiates DTT, Node, GroupNode &, EventNode types
		std::vector<kvProperty> kvData;
		uint32_t syncValue;
		std::vector<AnimDef> dttNodes;
		uint32_t tovrValue;
		std::vector<AnimDef> nodes;
		std::vector<std::string> descriptors;
		std::vector<GroupNode> groupNodes;

	};
};

using namespace StateNode;

class C_StateNode
{

private:
	std::istream* fs;

public:

	C_StateNode(std::istream* binaryStream) {
		this->fs = binaryStream;
	}

	enum {
		STAT = 0x74617473,
		KV__ = 0x5F5F766B,
		SYNC = 0x636E7973,
		DTT_ = 0x5F747464,
		TOVR = 0x72766F74,
		NODE = 0x65646F6E,
		DESC = 0x63736564,
		EVNT = 0x746E7665,
		TRIG = 0x67697274,
		ARGS = 0x73677261,
		GRP_ = 0x5F707267,
		MEMB = 0x626D656D,
		FRMS = 0x736D7266,
		CAND = 0x646E6163,
		SELS = 0x736C6573
	};

	void ReadDTTs() {
		std::vector<AnimDef> dttArray;
		uint32_t numDTTs = ReadUInt32(*fs);

		for (int i = 0; i < numDTTs; i++) {
			AnimDef dttNode;
			dttNode.streamType = DTT_;

			//do kvs
		}
	}

	void ReadSyncNode() {
		std::vector<SyncNode> syncNodes;
		uint32_t numSyncNodes = ReadUInt32(*fs);

		for (int i = 0; i < numSyncNodes; i++) {
			SyncNode sNode;
			sNode.valueA = ReadUInt64(*fs);
			sNode.valueB = ReadUInt64(*fs);
		}

		ReadUInt64(*fs); // 0x0 constant
	}

	void ConstructKVNode() {
		std::vector<kvProperty> kvProps;
		uint32_t numKVs = ReadUInt32(*fs);

		for (int i = 0; i < numKVs; i++) {
			kvProperty kvProp;
			kvProp.streamSig = ReadUInt32(*fs);

			// KV value indicates encoding. Document all todos.
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
					kvProp.chars = ReadString(*fs, ReadUInt32(*fs));
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

				default:
					kvProp.val32 = ReadUInt32(*fs);
					break;
			}

			kvProps.push_back(kvProp);
		}
	}

	void InitializeStateNode(std::vector<AnimDef>& nodes) {

		AnimDef adefNode;
		uint32_t stateNodeSig = ReadUInt32(*fs);
		uint32_t totalStateNodes = ReadUInt32(*fs);

		/* Iterate through all State Nodes. Assess stream format*/
		for (int i = 0; i < totalStateNodes; i++) {
			uint32_t streamType = ReadUInt32(*fs);

			switch (ntohl(streamType)) {
				case(KV__):
					ConstructKVNode();
					break;
				case(SYNC):
					ReadSyncNode();
					break;
				case(DTT_):
					ReadDTTs();
					break;
				case(TOVR):
					//todo RE
					break;
				case(NODE):
					break;
				case(DESC):
					break;
				case(EVNT):
					break;
			}

		}

		uint32_t groupNodeSig = ReadUInt32(*fs);
		uint32_t totalGroupNodes = ReadUInt32(*fs);

		/* Iterate through all Group Nodes. Assess stream formats*/
		for (int i = 0; i < totalGroupNodes; i++) {
			uint32_t streamType = ReadUInt32(*fs);

			switch (streamType) {
			case(DESC):
				break;
			}
		}
	}

};

