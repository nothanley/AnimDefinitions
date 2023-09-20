#include "C_DefStream.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma once

namespace StateNode {

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

	struct Node {
		uint32_t nodeType; // Differentiates DTT, Node, GroupNode &, EventNode types
		std::vector<KeyValueProp> kvProps;
		std::vector<Node> transNodes;
		std::vector<Node> nodes;
		std::vector<std::string> descriptors;
		std::vector<GroupNode> groupNodes;
	};

	struct KeyValueProp {
		uint32_t streamKey;
		std::string chars;
		uint64_t val64;
		uint32_t val32;
		uint16_t val16;
		uint8_t val8;
		float valFloat;
		bool flag;
	};

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


};
