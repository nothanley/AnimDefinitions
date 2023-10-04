#ifndef DTMPSERIALIZER_H
#define DTMPSERIALIZER_H
#include "AnimDefinitions/C_DefinitionEncoder.h"
#include <sstream>
#pragma comment(lib, "Ws2_32.lib")
#pragma once

using namespace BinaryIO;
using namespace std;

struct DefEntry{
    std::string name;
    uint64_t size = 0;
    uint64_t offset;
    std::vector<char> stream;
};

class DtmpSerializer
{
    enum {
        DTMP = 0x504D5444
    };

public:
    uint32_t numTemplates;
    bool isValid = false;

    DtmpSerializer(){
        this->isValid = false;
    }

    DtmpSerializer(std::string path){
        this->filePath = path;
        this->inFs = new ifstream(this->filePath, ios::binary);
        GetAllTemplates();
        this->inFs->close();
        delete this->inFs;
    }

    static void EncodeDataToVector(std::vector<char>* data, StateNode::Definition* definition){
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

    static void RemoveMatchingDefs(std::vector<DefEntry>* entries, std::string itemName){
        entries->erase(std::remove_if(entries->begin(), entries->end(),
                [&itemName](const DefEntry& entry) { return entry.name == itemName; }),
                entries->end());
    }

    void AppendDefinition(StateNode::Definition* definition, std::string name){
        RemoveMatchingDefs(&this->m_Templates,name); //clear any matching existing entries
        DefEntry serializedDef;
        EncodeDataToVector(&serializedDef.stream,definition);
        serializedDef.size = serializedDef.stream.size();
        serializedDef.name = name;
        this->m_Templates.push_back(serializedDef);
        this->numTemplates = this->m_Templates.size(); // update size
    }

    void EncodeFileTable(){
        for (auto &entry:this->m_Templates){
            WriteUInt64(outFs,entry.size);
            WriteUInt16(outFs,entry.name.size());
            WriteString(outFs,entry.name);  }
        for (auto &entry:this->m_Templates)
            outFs->write(entry.stream.data(),entry.size);
    }

    void EncodeHeader(){
        WriteUInt32(outFs,uint32_t(DTMP));
        WriteUInt32(outFs,this->numTemplates);
        EncodeFileTable();
    }

    void WriteContents(){
        ofstream outfile(filePath.c_str(), ios::binary);
        if (!outfile) { cout << "Invalid outfile"; return;} // Check if valid file
        this->outFs = &outfile;
        EncodeHeader();
        this->outFs->close();
    }

    std::vector<DefEntry> getTemplates(){ return this->m_Templates;}
private:
    std::ifstream* inFs;
    std::string filePath;
    std::ofstream* outFs;
    std::vector<DefEntry> m_Templates;

    bool IsValidFile(){
        if (!inFs->good()){ return false;}
        uint32_t fileMagic = ReadUInt32(*inFs);
        this->numTemplates = ReadUInt32(*inFs);
        if (fileMagic == DTMP){ return true;}
        return false;
    }

    void CollectDefinitionStreams(){
        for (int i = 0; i < numTemplates; i++){
            DefEntry* definition = &this->m_Templates.at(i);
            definition->stream.resize(definition->size);
            inFs->read(definition->stream.data(),definition->size); }
    }

    void CollectTemplate(DefEntry* definition){
        definition->size = ReadUInt64(*inFs);
        uint16_t charSize =  ReadUShort(*inFs)+1;
        definition->name = ReadString(*inFs, charSize);
    }

    void GetAllTemplates(){
        if (!IsValidFile()){return;}
        this->isValid = true;
        inFs->seekg(0x8);
        for (int i = 0; i < numTemplates; i++){
            DefEntry definition;
            CollectTemplate(&definition);
            this->m_Templates.push_back(definition); }
        CollectDefinitionStreams();
    }

    uint32_t getFileSize(std::ifstream& fileStream)
    {
        std::streampos currentPosition = fileStream.tellg();
        fileStream.seekg(0, std::ios::end);
        std::streampos fileSize = fileStream.tellg();
        fileStream.seekg(currentPosition, std::ios::beg);
        return uint32_t(fileSize);
    }


};

#endif // DTMPSERIALIZER_H
