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

    struct EventArgument {
        uint16_t index;
    };

    struct EventTrigger {
        uint64_t hash;
        float value;
    };

    struct EventNode {
        uint64_t value_0;
        uint32_t value_1;
        bool flag;
        uint32_t value_2;
        uint64_t value_3;
        std::vector<EventTrigger> triggers;
        std::vector<EventArgument> arguments;
    };

	struct SyncNode {
		uint64_t valueA;
		uint64_t valueB;
        bool flag;
	};

	struct Candidate {
        uint32_t type;
		uint64_t value;
		bool flag;

		float fValue_0;
		float fValue_1;
		uint16_t sValue_0;
	};

	struct Frame {
		uint64_t value;
		bool flag;
	};

	struct MemberNode {
		uint32_t type;
        std::vector<uint64_t> values;

		std::vector<Frame> frames;
		std::vector<Candidate> candidates;

        bool encodeFlag = false;
		uint32_t dValue_0;
		uint64_t lValue_0;
	};

	struct SelectorNode {
		std::vector<MemberNode> members;
	};

	struct GroupNode {
		std::string chars;
		std::vector<std::string> descriptors;
		MemberNode members;
		SelectorNode selectors;
	};

	struct Node {
		uint32_t nodeType; // Differentiates DTT, Node, GroupNode &, EventNode types
        std::vector<KeyValueProp> keyValueProperties;
		std::vector<SyncNode> syncNodes;
		std::vector<Node> transNodes;
		std::vector<Node> ovlyNodes;
		std::vector<Node> childNodes;
		std::vector<std::string> descriptors;
        std::vector<EventNode> events;
        uint8_t streamType;
        bool isBargNode = false;
        int32_t value_0;
        uint64_t value_1;
        bool flag = false;
        float floatVal;
	};

	struct Definition {
		std::vector<Node> stateNodes;
		std::vector<GroupNode> groupNodes;
	};

};
