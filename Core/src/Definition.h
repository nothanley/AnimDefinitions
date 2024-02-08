/* Parses XML ProjBin streams, populates all nodes with decoded type values */
#include <string>
#include <vector>
#pragma once

using namespace std;

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
class ProjectDefs
{

public:

	ProjectDefs(std::istream* stream);

	/* Reads through a definition binary */
	void ParseProjectXML(int binCount);

private:

	std::istream* fs;
	std::vector<XMLNode> m_XMLTrees;

	/* Populates definition tree*/
	void ReadProjectBin();

	/* Recursively gathers all def nodes */
	void ReadXMLNode(XMLNode& parent);

	void ReadCharacterNode(XMLNode& node);

	void ReadPropertyNode(XMLNode& node, std::vector<XMLProperty>& properties);

	void ReadElementNode(XMLNode& node);


};
