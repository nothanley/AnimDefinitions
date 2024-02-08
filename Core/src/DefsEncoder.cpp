#include "DefsEncoder.h"
//#include <QDebug>
#include "BinaryIO.h"
#include "Compression/zcompress.h"
#include <iostream>
#include <winsock.h>

using namespace BinaryIO;
using namespace std;

void CDefinitionEncoder::WriteToFileWithStream(string filePath, std::vector<char>* stream /* Includes all non adef buffers */){
    ofstream outfile(filePath.c_str(), ios::binary);
    if (!outfile) { cout << "Invalid outfile"; return;} // Check if valid file
    this->fs = &outfile;
    fs->write(stream->data(),stream->size());
    EncodeAllDefinitions();
    this->fs->close();
    //qDebug() << "compressing...";
    ADef file(filePath.c_str());
}

void CDefinitionEncoder::WriteToFile(string filePath){
    ofstream outfile(filePath.c_str(), ios::binary);
    if (!outfile) { cout << "Invalid outfile"; return;} // Check if valid file
    this->fs = &outfile;
    EncodeAllDefinitions();
    this->fs->close();
}


void EncodeStreamMagic(ofstream* stream, string magic, uint32_t defCount){
    WriteSignature(stream,magic);
    WriteUInt32(stream,defCount);
}

void EncodeKeyValueStream(ofstream* stream, vector<StateNode::KeyValueProp> properties, bool isNodeValue=false){
    EncodeStreamMagic(stream,"kv__",properties.size());
    for (auto const& prop : properties){
        CDefinitionEncoder::WriteNodeKey_State(stream,prop,isNodeValue);
    }
}

void EncodeSyncNode(ofstream* stream,StateNode::Node definition){
    EncodeStreamMagic(stream,"sync",definition.syncNodes.size());
    for (auto const& node : definition.syncNodes){
        WriteUInt64(stream,node.valueA);
        WriteUInt64(stream,node.valueB);
        WriteBool(stream,node.flag);
    }
    WriteUInt64(stream,definition.syncGlobal);
}

void EncodeDTTs(ofstream* stream,std::vector<Node> dttNodes){
    EncodeStreamMagic(stream,"dtt_",dttNodes.size());
    for (auto const& node : dttNodes){
        EncodeKeyValueStream(stream,node.keyValueProperties);
    }
}

void EncodeTOVR(ofstream* stream,std::vector<Node> tovrNodes){
    EncodeStreamMagic(stream,"tovr",tovrNodes.size());
    for (auto const& node : tovrNodes){
        EncodeKeyValueStream(stream,node.keyValueProperties);
    }
}

void EncodeTriggers(ofstream* stream, std::vector<EventTrigger> trigs){
    EncodeStreamMagic(stream,"trig",trigs.size());
    for (auto const& trig : trigs){
        WriteUInt64(stream,trig.hash);
        WriteFloat(stream,trig.value);  }
}

void EncodeArguments(ofstream* stream, std::vector<EventArgument> args){
    EncodeStreamMagic(stream,"args",args.size());
    for (auto const& arg : args)
        WriteUInt16(stream,arg.index);
}

void EncodeEvents(ofstream* stream, std::vector<EventNode> events){
    EncodeStreamMagic(stream,"evnt",events.size());
    for (auto const& event : events){
        WriteUInt64(stream,event.value_0);
        WriteUInt32(stream,event.value_1);
        WriteBool(stream,event.flag);
        if (!event.flag){
            WriteUInt32(stream,event.value_2);
            WriteUInt64(stream,event.value_3); }
        EncodeTriggers(stream,event.triggers);
        EncodeArguments(stream,event.arguments);    }
}


void EncodeBargStream(ofstream* stream,Node node){
    EncodeStreamMagic(stream,"barg",node.childNodes.size());
    for (auto const& child : node.childNodes){
        WriteUInt64(stream,child.barg_longlong);
        WriteFloat(stream,child.barg_float);
        CDefinitionEncoder::WriteNodeStream(stream,child);
    }
}

void EncodeChildNode(ofstream* stream, std::vector<Node> nodes){
    for (auto const& node : nodes){
        CDefinitionEncoder::WriteNodeStream(stream,node);
    }
}

void EncodeNodeStream(ofstream*stream,vector<Node> nodes){
    WriteSignature(stream,"node");
    EncodeChildNode(stream,nodes);
}

void EncodeDescriptors(ofstream* stream, std::vector<string> descs){
    EncodeStreamMagic(stream,"desc",descs.size());
    for (auto const& desc : descs)
        WriteChars(stream,desc);
}


void WriteNodeType1(ofstream* stream, Node node){
    WriteBool(stream,node.flag);
    EncodeKeyValueStream(stream,node.keyValueProperties,true);
}

void WriteNodeType2(ofstream* stream, Node node){
    WriteInt32(stream,node.value_0);
    EncodeKeyValueStream(stream,node.keyValueProperties,true);
    WriteUInt64(stream,node.value_1);
    WriteFloat(stream,node.floatVal);
    EncodeBargStream(stream,node);
}

void CDefinitionEncoder::WriteNodeStream(ofstream* stream, Node node){
    WriteUInt32(stream,node.streamType);
    switch(node.streamType){
    case(0x2):
        WriteNodeType2(stream,node);
        break;
    case(0x1):
        WriteNodeType1(stream,node);
        break;}
}

void EncodeStateNode(ofstream* stream,StateNode::Node state){
    EncodeKeyValueStream(stream,state.keyValueProperties);
    EncodeSyncNode(stream,state);
    EncodeDTTs(stream,state.transNodes);
    EncodeTOVR(stream,state.ovlyNodes);
    EncodeNodeStream(stream,state.childNodes);
    EncodeDescriptors(stream,state.descriptors);
    EncodeEvents(stream,state.events);
}

void EncodeFRMSStream(ofstream* stream, MemberNode member ){
    EncodeStreamMagic(stream,"frms",member.frames.size());
    for (auto const& frame : member.frames){
        WriteUInt64(stream,frame.value);
        WriteBool(stream,frame.flag);
    }
}

void EncodeCandidateStream(ofstream* stream, MemberNode member ){
    EncodeStreamMagic(stream,"cand",member.candidates.size());
    for (auto const& candidate : member.candidates){
        WriteUInt64(stream,candidate.value);
        WriteBool(stream,candidate.flag);    }
    CDefinitionEncoder::WriteCandidateStream(stream,member);
}

void EncodeMembers(ofstream* stream,MemberNode member){
    EncodeStreamMagic(stream,"memb",member.values.size());
    for (auto const& value : member.values)
        WriteUInt64(stream,value);

    WriteUInt32(stream,member.type);
    EncodeFRMSStream(stream,member);
    EncodeCandidateStream(stream,member);
}

void EncodeSelectors(ofstream* stream,SelectorNode selector){
    EncodeStreamMagic(stream,"sels",selector.members.size());
    for (auto const& member : selector.members){
        WriteUInt32(stream,member.type);
        EncodeFRMSStream(stream,member);
        EncodeCandidateStream(stream,member); }
}


void EncodeGroupNode(ofstream* stream,StateNode::GroupNode group){
    WriteChars(stream,group.chars);
    EncodeDescriptors(stream,group.descriptors);
    EncodeMembers(stream,group.members);
    EncodeSelectors(stream,group.selectors);
}

void CDefinitionEncoder::EncodeDefinition(ofstream* stream,StateNode::Definition definition){
    EncodeStreamMagic(stream,"stat", definition.stateNodes.size());
    for (auto const& state : definition.stateNodes){
        EncodeStateNode(stream,state);  }
    EncodeStreamMagic(stream,"grp_", definition.groupNodes.size());
    for (auto const& group : definition.groupNodes){
        EncodeGroupNode(stream,group);  }
}

void CDefinitionEncoder::EncodeAllDefinitions(){
    EncodeStreamMagic(fs,"adef",this->m_Definitions->size());
    /* Iterate through all defs and encode */
    for (int i = 0; i < m_Definitions->size(); i++){
        EncodeDefinition(fs,m_Definitions->at(i));  }
}


void CDefinitionEncoder::WriteCandidateStream(ofstream* stream, StateNode::MemberNode member){

    switch (ntohl(member.type)) {
        case(0xEB7AD409):
            for (auto const& candidate : member.candidates){
                WriteFloat(stream,candidate.fValue_0);
                WriteFloat(stream,candidate.fValue_1); }
            WriteUInt32(stream,member.dValue_0);
            break;
        case(0x76A50FDC):
            WriteUInt64(stream,member.lValue_0);
            WriteBool(stream,member.encodeFlag);
            break;
        case(0x4BFF8647):
            for (auto const& candidate : member.candidates)
                WriteFloat(stream,candidate.fValue_0);
            WriteBool(stream,member.encodeFlag);
            break;
        case(0x2F05210F):
            for (auto const& candidate : member.candidates){
                WriteUInt16(stream,candidate.sValue_0);
                WriteFloat(stream,candidate.fValue_0);
                WriteFloat(stream,candidate.fValue_1);  }
            WriteUInt64(stream,member.lValue_0);
            WriteBool(stream,member.encodeFlag);
            break;
        case(0x1A8AECAF):
            for (auto const& candidate : member.candidates)
                WriteUInt16(stream,candidate.sValue_0);
            WriteUInt64(stream,member.lValue_0);
            WriteBool(stream,member.encodeFlag);
            break;
        default:
            break;
    }

}
void CDefinitionEncoder::WriteNodeKey_State(ofstream* stream, StateNode::KeyValueProp property, bool isNodeValue){
    WriteUInt32(stream,property.streamKey);
    // This switch is veeery long...
    switch (ntohl(property.streamKey)) {
        case (0x46C0C6BF):
            WriteBool(stream,property.flag);
            break;
        case (0xA8FA930E):
            WriteBool(stream,property.flag);
            break;
        case (0x4D245F2B):
            WriteBool(stream,property.flag);
            break;
        case (0x6782648B):
            WriteBool(stream,property.flag);
            break;
        case (0xDF4B0B8E):
            if (!isNodeValue) { WriteChars(stream,property.chars); }
            else { WriteUInt64(stream,property.val64); }
            break;
        case (0x60C09DC4):
            WriteBool(stream,property.flag);
            break;
        case (0xA6D0A4EB):
            WriteBool(stream,property.flag);
            break;
        case (0x5D651EEF):
            WriteBool(stream,property.flag);
            break;
        case (0x4425DADA):
            WriteUInt32(stream,property.val32);
            break;
        case (0x45991D01):
            WriteChars(stream,property.chars);
            break;
        case (0x48DE1308):
            WriteBool(stream,property.flag);
            break;
        case (0x73EDDE33):
            WriteUInt64(stream,property.val64);
            break;
        case (0x0C3F823A):
            WriteBool(stream,property.flag);
            break;
        case (0x654680ED):
            WriteUInt32(stream,property.val32);
            break;
        case (0xC248BCF4 ):
            WriteBool(stream,property.flag);
            break;
        case (0x8AE38AFE):
            WriteUInt32(stream,property.val32);
            break;
        case (0x8E0B4BDF):
            WriteUInt64(stream,property.val64);
            break;
        case (0xBE83F7FD):
            WriteChars(stream,property.chars);
            break;
        case (0x78D548C7):
            WriteUInt32(stream,property.val32);
            break;
        case (0xC8B22C04):
            WriteFloat(stream,property.valFloat);
            break;
        case (0x19981A6F):
            WriteFloat(stream,property.valFloat);
            break;
        case (0x9E3F108C):
            WriteChars(stream,property.chars);
            break;
        case (0x4C980DCF):
            WriteFloat(stream,property.valFloat);
            break;
        case (0xE39D3DA5):
            WriteFloat(stream,property.valFloat);
            break;
        case (0x0A2E3AF4):
            WriteBool(stream,property.flag);
            break;

        default:
            std::cout << "\nInvalid KV value. Cannot KV stream.";
            break;
    }
}

