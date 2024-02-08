#ifndef DTMPSERIALIZER_H
#define DTMPSERIALIZER_H
#include "StateNode.h"
#include "DefsEncoder.h"
#pragma once

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
    DtmpSerializer();
    DtmpSerializer(std::string path);

    static void EncodeDataToVector(std::vector<char>* data, StateNode::Definition* definition);
    static void RemoveMatchingDefs(std::vector<DefEntry>* entries, std::string itemName);
    void AppendDefinition(StateNode::Definition* definition, std::string name);
    void EncodeFileTable();
    void EncodeHeader();
    void WriteContents();
    std::vector<DefEntry> getTemplates();

    uint32_t numTemplates;
    bool isValid = false;

private:

    std::ifstream* inFs;
    std::string filePath;
    std::ofstream* outFs;
    std::vector<DefEntry> m_Templates;

    bool IsValidFile();
    void CollectDefinitionStreams();
    void CollectTemplate(DefEntry* definition);
    void GetAllTemplates();
    uint32_t getFileSize(std::ifstream& fileStream);


};

#endif // DTMPSERIALIZER_H
