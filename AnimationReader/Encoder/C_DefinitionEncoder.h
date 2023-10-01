#ifndef C_DEFINITIONENCODER_H
#define C_DEFINITIONENCODER_H
#include "AnimDefinitions/AnimDefHandler.h"

class CDefinitionEncoder
{

public:
    CDefinitionEncoder(std::vector<StateNode::Definition>* definitions){ this->m_Definitions = definitions;}
    void WriteToFile(std::string filePath);
    void WriteToFileWithStream(string filePath, std::vector<char>* stream);
    static void WriteNodeKey_State(ofstream* stream, StateNode::KeyValueProp property, bool isNodeValue=false);
    static void WriteNodeStream(ofstream* stream, Node node);
    static void WriteCandidateStream(ofstream* stream, StateNode::MemberNode member);
    static void EncodeDefinition(ofstream* stream,StateNode::Definition definition);
private:
    void EncodeAllDefinitions();
    std::ofstream* fs;
    std::vector<StateNode::Definition>* m_Definitions = nullptr;
};

#endif // C_DEFINITIONENCODER_H
