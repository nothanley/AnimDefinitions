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
	
	void ReadNodeType1() {
		bool nodeFlag = ReadBool(*fs);
		uint32_t streamSig = ReadUInt32(*fs); // Init KVs
		uint32_t numKVs = ReadUInt32(*fs);

		for (int i = 0; i < numKVs; i++) {
			kvProperty kvProp;
			kvProp.streamSig = ReadUInt32(*fs);

			switch (kvProp.streamSig) {
				case (0xC748D578):
					kvProp.val32 = ReadUInt32(*fs);
					break;
				case (0x42CB2C8):
					kvProp.valFloat = ReadFloat(*fs);
					break;
				case (0x6F1A9819):
					kvProp.valFloat = ReadFloat(*fs);
					break;
				case (0x8C103F9E):
					kvProp.chars = ReadString(*fs, ReadUInt32(*fs));
					break;
				case (0x8E0B4BDF):
					kvProp.val64 = ReadUInt64(*fs);
					break;


				case (0xCF0D984C):
					kvProp.valFloat = ReadFloat(*fs);
					break;
				case (0xE39D3DA5):
					kvProp.valFloat = ReadFloat(*fs);
					break;
				case (0xF43A2E0A):
					kvProp.val8 = ReadBool(*fs);
					break;
				case (0xFDF783BE):
					kvProp.chars = ReadString(*fs, ReadUInt32(*fs));
					break;

				default:
					std::cout << fs->tellg();
					std::cout << "\nInvalid KV value. Cannot KV stream.";
					break;
			}
		}
		
	}

	void ReadNodeType2() {
		int32_t nodeFlag = ReadSInt32(*fs);
		uint32_t streamSig = ReadUInt32(*fs);
		uint32_t numKVs = ReadUInt32(*fs);

		for (int i = 0; i < numKVs; i++) {
			kvProperty kvProp;
			kvProp.streamSig = ReadUInt32(*fs);

			switch (kvProp.streamSig) {
				case (0x3A823F0C):
					kvProp.val8 = ReadBool(*fs);
					break;
				case (0x8E0B4BDF):
					kvProp.val64 = ReadUInt64(*fs);
					break;
				case (0xFDF783BE):
					kvProp.chars = ReadString(*fs, ReadUInt32(*fs));
					break;

				default:
					std::cout << fs->tellg();
					std::cout << "\nInvalid KV value. Cannot KV stream.";
					break;	}
		}

		uint64_t unkVal64 = ReadUInt64(*fs);
		float unkFloat = ReadFloat(*fs);

		//Process 'BARG' stream
		uint32_t bargSig = ReadUInt32(*fs);
		uint32_t numBargs = ReadUInt32(*fs);

		for (int i = 0; i < numBargs; i++) {
			uint64_t unkVal64 = ReadUInt64(*fs);
			float unkFloat = ReadFloat(*fs);
			ProcessNode();
		}
	}

	void ProcessNode() {
		uint32_t nodeType = ReadUInt32(*fs);

		switch (nodeType) {
			case(0x2):
				ReadNodeType2();
				break;
			case(0x1):
				ReadNodeType1();
				break;
			default:
				std::cout << "\nInvalid stream type.";
				break;
		}
	}

	void ProcessDTTs() {
		std::vector<AnimDef> dttArray;
		uint32_t numDTTs = ReadUInt32(*fs);

		for (int i = 0; i < numDTTs; i++) {
			AnimDef dttNode;
			dttNode.streamType = DTT_;

			ReadUInt32(*fs); // Init KVs
			uint32_t numKVs = ReadUInt32(*fs);
			for (int j = 0; j < numKVs; j++) {
				kvProperty kvProp;
				kvProp.streamSig = ReadUInt32(*fs);

				switch (kvProp.streamSig) {
					case (0xDADA2544):
						kvProp.val32 = ReadUInt32(*fs);
						break;
					case (0x11D9945):
						kvProp.chars = ReadString(*fs, ReadUInt32(*fs));
						break;
					case (0x813DE48):
						kvProp.val8 = ReadBool(*fs);
						break;
					case (0x33DEED73):
						kvProp.val64 = ReadUInt64(*fs);
						break;
					case (0x3A823F0C):
						kvProp.val8 = ReadBool(*fs);
						break;


					case (0xED804665):
						kvProp.val32 = ReadUInt32(*fs);
						break;
					case (0xF4BC48C2):
						kvProp.val8 = ReadBool(*fs);
						break;
					case (0xFE8AE38A):
						kvProp.val32 = ReadUInt32(*fs);
						break;

					default:
						std::cout << fs->tellg();
						std::cout << "\nInvalid KV value. Cannot KV stream.";
						break;
				}
				dttNode.kvData.push_back(kvProp);
			}
		}
	}

	void ProcessSyncNode() {
		std::vector<SyncNode> syncNodes;
		uint32_t numSyncNodes = ReadUInt32(*fs);

		for (int i = 0; i < numSyncNodes; i++) {
			SyncNode sNode;
			sNode.valueA = ReadUInt64(*fs);
			sNode.valueB = ReadUInt64(*fs);
			sNode.flag = ReadBool(*fs);
		}

		ReadUInt64(*fs); // 0x0 constant
	}

	void ProcessDescriptor() {
		std::vector<std::string> descriptors;
		uint32_t numDescs = ReadUInt32(*fs);
		
		for (int i = 0; i < numDescs; i++) {
			std::string chars = ReadString(*fs, ReadUInt32(*fs));
			descriptors.push_back(chars);
		}
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
					std::cout << fs->tellg();
					std::cout << "\nInvalid KV value. Cannot KV stream.";
					break;
			}

			kvProps.push_back(kvProp);
		}
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

	void ReadCandStream( uint32_t candidateEncoding ) {
		ReadUInt32(*fs);
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
				std::cout << "\nCould not read cand node.";
				break;
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

	void InitializeStateNode(std::vector<AnimDef>& nodes) {
		// Reads all state nodes in adef stream. 
		// TODO: Cleanup. This works, but could flow better.

		AnimDef adefNode;
		uint32_t stateNodeSig = ReadUInt32(*fs);
		uint32_t totalStateNodes = ReadUInt32(*fs);

		/* Iterate through all State Nodes. Assess stream format*/
		for (int i = 0; i < totalStateNodes;) {
			uint32_t streamType = ReadUInt32(*fs);

			while (ntohl(streamType) != GRP_) {
				switch (ntohl(streamType)) {
					case(KV__):
						ConstructKVNode();
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
						ProcessNode();
						break;
					case(DESC):
						ProcessDescriptor();
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

};

