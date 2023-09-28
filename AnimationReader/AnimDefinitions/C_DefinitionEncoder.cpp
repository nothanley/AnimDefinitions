#include "C_DefinitionEncoder.h"

void CDefinitionEncoder::WriteToFile(std::string filePath){
    std::ofstream outfile(filePath.c_str(), std::ios::binary);

    // check if the file was opened successfully
    if (!outfile) { std::cout << "Invalid outfile"; return;}
    this->fs = &outfile;

    EncodeAllDefinitions();
    this->fs->close();
}

void WriteADefHeader(std::ofstream* stream, uint32_t defCount){
    WriteSignature(stream,"adef");
    WriteUInt32(stream,defCount);
}

void CDefinitionEncoder::EncodeAllDefinitions(){
    WriteADefHeader(fs,this->m_Definitions->size());
}



