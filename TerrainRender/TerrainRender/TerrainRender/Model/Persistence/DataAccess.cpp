#include "DataAccess.h"

#include <fstream>
#include <string>
#include <windows.h>
#include "ModelVertex.h"
#include <vector>


void TextFileDataAccess::CreateVertex(ModelVertex& vertex, std::stringstream& sstream, std::vector<ModelVertex>& vertices)
{
	std::string word;
	sstream >> word;

	if ("facet" == word)
	{
		vertex.Clear();
		sstream >> word; //normal
		sstream >> word; //x
		vertex.normal.x = std::stof(word);
		sstream >> word; //y
		vertex.normal.y = std::stof(word);
		sstream >> word; //z
		vertex.normal.z = std::stof(word);
	}
	else if ("vertex" == word)
	{
		sstream >> word; //x
		vertex.position.x = std::stof(word);
		sstream >> word; //y
		vertex.position.y = std::stof(word);
		sstream >> word; //z
		vertex.position.z = std::stof(word);
		vertices.push_back(vertex);
	}


}

bool TextFileDataAccess::LoadTerrain(const wchar_t* filename, std::vector<ModelVertex>& vertices)
{
	std::ifstream input;
	std::string line;
	input.open(filename, std::ifstream::in);

	if (!input.is_open())
		return false;

	ModelVertex vertex;
	vertices.clear();

	while (std::getline(input, line))
	{
		std::stringstream sstream(line);
		CreateVertex(vertex, sstream, vertices);
	}
	
	input.close();

	return true;

}

