#include "Definition.h"
#include <iostream>
#include "BinaryIO.h"
#include <vector>
#pragma comment(lib, "Ws2_32.lib")
#pragma once

using namespace BinaryIO;

ProjectDefs::ProjectDefs(std::istream* stream, std::vector<string> *pParentStrings) {
	this->fs = stream;
    this->m_pParentVec = pParentStrings;
}

void 
ProjectDefs::ParseProjectXML(int binCount) {
	for (int i = 0; i < binCount; i++) {
		ReadProjectBin();
	}
}

void 
ProjectDefs::ReadProjectBin() {
	uint16_t iProjectBinMagic = ReadUShort(*fs);
	uint16_t iProjectBinFlag = ReadUShort(*fs);

	XMLNode root;
	ReadXMLNode(root);
	this->m_XMLTrees.push_back(root);
	return;
}

void 
ProjectDefs::ReadXMLNode(XMLNode& parent) {
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

void 
ProjectDefs::ReadCharacterNode(XMLNode& node) {
	node.charSize = ReadByte(*fs);
	node.numProperties++;
	node.name = ReadString(*fs, node.charSize + 1);
}

void 
ProjectDefs::ReadPropertyNode(XMLNode& node, std::vector<XMLProperty>& properties) {
	node.numChildren = ReadByte(*fs);
	node.charSize = ReadUShort(*fs);
    XMLProperty property = XMLProperty{ ReadString(*fs, node.charSize + 1) };
    m_pParentVec->push_back(property.value);
    properties.push_back(property);
}

void 
ProjectDefs::ReadElementNode(XMLNode& node) {
	node.charSize = ReadByte(*fs);
	node.numChildren = ReadUShort(*fs);
	node.numProperties = ReadByte(*fs);
	node.name = ReadString(*fs, node.charSize + 1);
}



