// WIP Reads Nodes and properties within adef binary stream. 
// TODO: All streams are parsed, next cleanup the code and collect everything in proper objects.

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
		float valFloat;
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
		bool flag;

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
	
	void InitializeStateNode(std::vector<AnimDef>& nodes);
	AnimDef ReadNodeType1();
	AnimDef ReadNodeType2();
	std::vector<AnimDef> ProcessDTTs();
	std::vector<kvProperty> ReadKeyValueProperty(bool isNodeValue);
	void ReadCandStream(uint32_t candidateEncoding);

	AnimDef ProcessNode() {
		AnimDef parentNode{ NODE };
		uint32_t nodeType = ReadUInt32(*fs);

		switch (nodeType) {
			case(0x2):
				parentNode.nodes.push_back(ReadNodeType2());
				break;
			case(0x1):
				parentNode.nodes.push_back(ReadNodeType1());
				break;
			default:
				std::cout << "\nInvalid stream type.";
				break;
		}
		return parentNode;
	}

	
	std::vector<SyncNode> ProcessSyncNode() {
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

	std::vector<std::string> ProcessDescriptor() {
		std::vector<std::string> descriptors;
		uint32_t numDescs = ReadUInt32(*fs);
		
		for (int i = 0; i < numDescs; i++) {
			std::string chars = ReadString(*fs, ReadUInt32(*fs));
			descriptors.push_back(chars);
		}
		return descriptors;
	}

	void ReadEventArguments() {
		ReadUInt32(*fs); //  0xARGS
		uint32_t numArgs = ReadUInt32(*fs);
		for (int i = 0; i < numArgs; i++) {
			uint16_t argIdx = ReadUShort(*fs);
		}
	}

	void ReadEventTriggers() {
		ReadUInt32(*fs); // 0xTRIGS
		uint32_t numTriggers = ReadUInt32(*fs);
		for (int i = 0; i < numTriggers; i++) {
			uint64_t unkVal1 = ReadUInt64(*fs);
			float unkFloat1 = ReadFloat(*fs);
		}
	}
	
	void ProcessSMEvents() {
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
	
	void ReadFRMSStream() {
		ReadUInt32(*fs);
		uint32_t numFrames = ReadUInt32(*fs);

		for (int i = 0; i < numFrames; i++) {
			uint64_t unkVal1 = ReadUInt64(*fs);
			bool flag = ReadBool(*fs);
		}
	}

	void ProcessMembers() {
		uint32_t numMembs = ReadUInt32(*fs);
		uint32_t selType;

		for (int i = 0; i < numMembs; i++)
			uint64_t unkVal1 = ReadUInt64(*fs);

		selType = ReadUInt32(*fs);

		ReadFRMSStream();
		ReadCandStream(selType);
	}

	void ProcessSelectors() {
		uint32_t numSels = ReadUInt32(*fs);
		uint32_t selType;

		for (int i = 0; i < numSels; i++) {
			selType = ReadUInt32(*fs);
			ReadFRMSStream();
			ReadCandStream(selType); 
		}
	}


};

