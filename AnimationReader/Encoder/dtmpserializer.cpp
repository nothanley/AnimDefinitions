#include "dtmpserializer.h"
#include "BinaryIO.h"
#include <sstream>
#include <iostream>

using namespace BinaryIO;
using namespace std;

DtmpSerializer::DtmpSerializer(){
    this->isValid = false;
}

DtmpSerializer::DtmpSerializer(std::string path){
    this->filePath = path;
    this->inFs = new ifstream(this->filePath, ios::binary);
    GetAllTemplates();
    this->inFs->close();
    delete this->inFs;
}

void
DtmpSerializer::EncodeDataToVector(std::vector<char>* data, StateNode::Definition* definition){
    std::stringstream stream;
    std::ofstream outputFile;
    outputFile.set_rdbuf(stream.rdbuf());
    CDefinitionEncoder::EncodeDefinition(&outputFile,*definition);
    std::string binary = stream.str();
    std::vector<char> streamData;
    stream.seekg(0);
    streamData.insert(streamData.begin(), binary.begin(), binary.end());
    streamData.resize(binary.size());
    *data = streamData;
}

void
DtmpSerializer::RemoveMatchingDefs(std::vector<DefEntry>* entries, std::string itemName){
    entries->erase(std::remove_if(entries->begin(), entries->end(),
            [&itemName](const DefEntry& entry) { return entry.name == itemName; }),
            entries->end());
}

void
DtmpSerializer::AppendDefinition(StateNode::Definition* definition, std::string name){
    RemoveMatchingDefs(&this->m_Templates,name); //clear any matching existing entries
    DefEntry serializedDef;
    EncodeDataToVector(&serializedDef.stream,definition);
    serializedDef.size = serializedDef.stream.size();
    serializedDef.name = name;
    this->m_Templates.push_back(serializedDef);
    this->numTemplates = this->m_Templates.size(); // update size
}

void
DtmpSerializer::EncodeFileTable(){
    for (auto &entry:this->m_Templates){
        WriteUInt64(outFs,entry.size);
        WriteUInt16(outFs,entry.name.size());
        WriteString(outFs,entry.name);  }
    for (auto &entry:this->m_Templates)
        outFs->write(entry.stream.data(),entry.size);
}

void
DtmpSerializer::EncodeHeader(){
    WriteUInt32(outFs,uint32_t(DTMP));
    WriteUInt32(outFs,this->numTemplates);
    EncodeFileTable();
}

void
DtmpSerializer::WriteContents(){
    ofstream outfile(filePath.c_str(), ios::binary);
    if (!outfile) { std::cout << "Invalid outfile"; return;} // Check if valid file
    this->outFs = &outfile;
    EncodeHeader();
    this->outFs->close();
}

std::vector<DefEntry>
DtmpSerializer::getTemplates(){
    return this->m_Templates;
}

bool
DtmpSerializer::IsValidFile(){
    if (!inFs->good()){ return false;}
    uint32_t fileMagic = ReadUInt32(*inFs);
    this->numTemplates = ReadUInt32(*inFs);
    if (fileMagic == DTMP){ return true;}
    return false;
}

void
DtmpSerializer::CollectDefinitionStreams(){
    for (int i = 0; i < numTemplates; i++){
        DefEntry* definition = &this->m_Templates.at(i);
        definition->stream.resize(definition->size);
        inFs->read(definition->stream.data(),definition->size); }
}

void
DtmpSerializer::CollectTemplate(DefEntry* definition){
    definition->size = ReadUInt64(*inFs);
    uint16_t charSize =  ReadUShort(*inFs)+1;
    definition->name = ReadString(*inFs, charSize);
}

void
DtmpSerializer::GetAllTemplates(){
    if (!IsValidFile()){return;}
    this->isValid = true;
    inFs->seekg(0x8);
    for (int i = 0; i < numTemplates; i++){
        DefEntry definition;
        CollectTemplate(&definition);
        this->m_Templates.push_back(definition); }
    CollectDefinitionStreams();
}

uint32_t
DtmpSerializer::getFileSize(std::ifstream& fileStream)
{
    std::streampos currentPosition = fileStream.tellg();
    fileStream.seekg(0, std::ios::end);
    std::streampos fileSize = fileStream.tellg();
    fileStream.seekg(currentPosition, std::ios::beg);
    return uint32_t(fileSize);
}
