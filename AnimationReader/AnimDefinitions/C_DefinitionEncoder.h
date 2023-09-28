#ifndef C_DEFINITIONENCODER_H
#define C_DEFINITIONENCODER_H
#include "AnimDefHandler.h"

class CDefinitionEncoder
{

public:
    CDefinitionEncoder(std::vector<StateNode::Definition>* definitions){ this->m_Definitions = definitions;}
    void WriteToFile(std::string filePath);

private:


    void EncodeAllDefinitions();
    std::ofstream* fs;
    std::vector<StateNode::Definition>* m_Definitions = nullptr;
};

#endif // C_DEFINITIONENCODER_H
