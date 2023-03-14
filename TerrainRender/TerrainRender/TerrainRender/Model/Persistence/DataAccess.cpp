#include "DataAccess.h"

#include <fstream>
#include <string>
#include <windows.h>
#include "ModelStructs.h"
#include <vector>
#include <regex>


bool TextFileDataAccess::CreateVertex(ModelVertex& vertex,const std::string& line,std::vector<ModelVertex>& vertices, STLLineType& type, int& vertexCount) {
    bool bresult;
    switch (type) {
    case BEGIN: {
        bresult = std::regex_match(line, std::regex("solid .+"));
        type = FACET;
        return bresult;
    }
    case FACET:
    case END: {
        vertex.Clear();

        bresult = std::regex_match(line, std::regex("endsolid .+"));
        if (bresult)
        {
            return true;
        }
        bresult = std::regex_match(
            line, std::regex("[ \t\r\n\f]*facet normal( -?[0-9]+[.]?[0-9]*(e-[0-9]+)?){3}"));
        if (!bresult) {
            return false;
        }
        std::stringstream sstream(line);
        std::string word;
        sstream >> word; // facet
        sstream >> word; // normal
        sstream >> word; // x
        vertex.normal.x = std::stof(word);
        sstream >> word; // y
        vertex.normal.y = std::stof(word);
        sstream >> word; // z
        vertex.normal.z = std::stof(word);
        type = LOOP;
        return true;
    }
    case LOOP: {
        bresult = std::regex_match(line, std::regex("[ \t\r\n\f]*outer loop"));
        type = VERTEX;
        return bresult;
    }
    case VERTEX: {
        bresult = std::regex_match(
            line, std::regex("[ \t\r\n\f]*vertex( -?[0-9]+[.]?[0-9]*(e-[0-9]+)?){3}"));
        if (!bresult) {
            return false;
        }
        std::stringstream sstream(line);
        std::string word;
        sstream >> word; // vertex
        sstream >> word; // x
        vertex.position.x = std::stof(word);
        sstream >> word; // y
        vertex.position.y = std::stof(word);
        sstream >> word; // z
        vertex.position.z = std::stof(word);
        vertices.push_back(vertex);

        if (vertexCount < 2) {
            vertexCount++;
            type = VERTEX;
        }
        else if (vertexCount == 2) {
            vertexCount = 0;
            type = ENDLOOP;
        }
        return bresult;
    }
    case ENDLOOP: {
        bresult = std::regex_match(line, std::regex("[ \t\r\n\f]*endloop"));
        type = ENDFACET;
        return bresult;
    }
    case ENDFACET: {
        bresult = std::regex_match(line, std::regex("[ \t\r\n\f]*endfacet"));
        type = FACET;
        return bresult;
    }
    }
}

bool TextFileDataAccess::LoadTerrain(const wchar_t* filename, std::vector<ModelVertex>& vertices)
{
	std::ifstream   input;
	std::string     line;
    STLLineType     type = STLLineType::BEGIN;
	ModelVertex     vertex;

	input.open(filename, std::ifstream::in);

	if (!input.is_open())
		return false;

	vertices.clear();

    bool isCorrect = true;
    int vertexCount = 0;
    while ((std::getline(input, line)) && isCorrect) 
    {
        isCorrect = CreateVertex(vertex, line, vertices, type, vertexCount);
    }
    if (!isCorrect) 
    {
        vertices.clear();
    }

    input.close();

    return isCorrect;
}

