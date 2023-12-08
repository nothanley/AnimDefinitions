/* Parser for project binaries */
#pragma once
#include <iostream>
#include "BinaryIO.h"
#include <vector>
#pragma comment(lib, "Ws2_32.lib")
#pragma once


namespace DefNode {
struct XMLProperty {
    std::string value;
};

struct XMLNode {
    uint16_t streamSize = 0;
    uint16_t numChildren = 0;
    uint16_t numProperties = 0;
    uint16_t charSize = 0;
    uint16_t index = 0;
    std::string name;
    std::vector<XMLNode> children;
    std::vector<XMLProperty> properties;
};
}

using namespace DefNode;
using namespace std;
using namespace BinaryIO;

class ProjectDefs
{

public:

    ProjectDefs(std::istream* stream, std::vector<std::string>* pParentStrings) {
        this->fs = stream;
        this->m_pParentVec = pParentStrings;
    }

    /* Reads through a definition binary */
    void ParseProjectXML(int binCount) {
        for (int i = 0; i < binCount; i++) {
            ReadProjectBin();
        }
    }

private:
    std::istream* fs;
    std::vector<XMLNode> m_XMLTrees;
    std::vector<std::string>* m_pParentVec;

    /* Populates definition tree*/
    void ReadProjectBin() {
        uint16_t iProjectBinMagic = ReadUShort(*fs);
        uint16_t iProjectBinFlag = ReadUShort(*fs);

        XMLNode root;
        ReadXMLNode(root);
        this->m_XMLTrees.push_back(root);
        return;
    }

    /* Recursively gathers all def nodes */
    void ReadXMLNode(XMLNode& parent) {
        XMLNode node;
        uint8_t nodeType;

        node.streamSize = ReadUShort(*fs);
        nodeType = ReadByte(*fs);

        switch (nodeType) {
        case 0x5:
            ReadCharacterNode(node);
            break;
        case 0x4:
            ReadPropertyNode(node, parent.properties);
            return;
        case 0x1:
            ReadElementNode(node);
            break;
        }

        for (int i = 0; i < node.numChildren; i++) {
            ReadXMLNode(node);
        }
        for (int i = 0; i < node.numProperties; i++) {
            ReadXMLNode(node);
        }

        parent.children.push_back(node);
    }

    void ReadCharacterNode(XMLNode& node) {
        node.charSize = ReadByte(*fs);
        node.numProperties++;
        node.name = ReadString(*fs, node.charSize + 1);
    }

    void ReadPropertyNode(XMLNode& node, std::vector<XMLProperty>& properties) {
        node.numChildren = ReadByte(*fs);
        node.charSize = ReadUShort(*fs);

        std::string name = ReadString(*fs, node.charSize + 1);
        m_pParentVec->push_back(name);

        properties.push_back(XMLProperty{ name });
    }

    void ReadElementNode(XMLNode& node) {
        node.charSize = ReadByte(*fs);
        node.numChildren = ReadUShort(*fs);
        node.numProperties = ReadByte(*fs);
        node.name = ReadString(*fs, node.charSize + 1);
    }


};
