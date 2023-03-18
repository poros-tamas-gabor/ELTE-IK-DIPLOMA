#include "DataAccess.h"

#include <fstream>
#include <string>
#include <windows.h>
#include "ModelStructs.h"
#include <vector>
#include <regex>


bool TextFileDataAccess::CreateVertex(Vertex& vertex,const std::string& line,std::vector<Vertex>& vertices, STLLineType& type, int& vertexCount) {
    bool bresult;
    switch (type) {
    case BEGIN: {
        //bresult = std::regex_match(line, std::regex("solid.+"));
        type = FACET;
        //return bresult;
        return true;
    }
    case FACET:
    case END: {
        vertex.color    = { 1.0f, 1.0f, 1.0f, 1.0f };
        vertex.normal   = { 0,0,0};
        vertex.position = { 0,0,0 };

       bresult = std::regex_match(line, std::regex("endsolid.+"));
       if (bresult)
       {
           return true;
       }
       // bresult = std::regex_match(
        //    line, std::regex("[ \t\r\n\f]*facet[ \t\r\n\f]*normal([ \t\r\n\f]*-?[0-9]+[.]?[0-9]*(e[+-][0-9]+)?){3}"));
        //if (!bresult) {
        //    return false;
        //}
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
        //bresult = std::regex_match(line, std::regex("[ \t\r\n\f]*outer[ \t\r\n\f]*loop"));
        type = VERTEX;
        //return bresult;
        return true;
    }
    case VERTEX: {
       //bresult = std::regex_match(
       //    line, std::regex("[ \t\r\n\f]*vertex([ \t\r\n\f]*-?[0-9]+[.]?[0-9]*(e[+-][0-9]+)?){3}"));
        //if (!bresult) {
        //    return false;
        //}
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
        //return bresult;
        return true;
    }
    case ENDLOOP: {
        //bresult = std::regex_match(line, std::regex("[ \t\r\n\f]*endloop"));
        type = ENDFACET;
        return true;
        //return bresult;
    }
    case ENDFACET: {
        //bresult = std::regex_match(line, std::regex("[ \t\r\n\f]*endfacet"));
        type = FACET;
        //return bresult;
        return true;
    }
    }
}

bool TextFileDataAccess::LoadTerrain(const wchar_t* filename, std::vector<Vertex>& vertices)
{
	std::ifstream   input;
	std::string     line;
    STLLineType     type = STLLineType::BEGIN;
	Vertex          vertex;

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


bool TextFileDataAccess::CreateCameraPose(CameraPose& cameraPose, const std::string& line, const std::vector<std::string>& headers) 
{
    //bool bresult = std::regex_match(line, std::regex("(-?[0-9]+[.]?[0-9]*(e[+-][0-9]+)?)(;(-?[0-9]+[.]?[0-9]*(e[+-][0-9]+)?))*"));
   //if (!bresult)
   //{
   //    return false;
   //}

    std::istringstream lineStream(line);
    std::string word;

    int i = -1;
    while ((std::getline(lineStream, word, ';'))) {
        i++;
        if (headers.at(i) == "sec") {
            cameraPose.sec = std::stoi(word);
        }
        else if (headers.at(i) == "nsec") {
            cameraPose.nsec = std::stoi(word);
        }
        else if (headers.at(i) == "yaw") {
            cameraPose.yaw = std::stof(word);
        }
        else if (headers.at(i) == "pitch") {
            cameraPose.pitch = std::stof(word);
        }
        else if (headers.at(i) == "roll") {
            cameraPose.roll = std::stof(word);
        }
        else if (headers.at(i) == "north") {
            cameraPose.north = std::stof(word);
        }
        else if (headers.at(i) == "east") {
            cameraPose.east = std::stof(word);
        }
        else if (headers.at(i) == "down") {
            cameraPose.down = std::stof(word);
        }
    }

    return true;
}

bool TextFileDataAccess::LoadCameraTrajectory( const wchar_t* filename, std::vector<CameraPose>& cameraPoses) 
{
    std::ifstream            input;
    std::string              line;
    std::string              word;
    std::vector<std::string> headers;
    CameraPose               cameraPose;

    input.open(filename, std::ifstream::in);

    if (!input.is_open())
        return false;

    cameraPoses.clear();

    bool isCorrect = true;
    // read first line
    std::getline(input, line);

    //read the headers
    std::istringstream lineStream(line);
    while ((std::getline(lineStream, word, ';'))) {
        headers.push_back(word);
    }

    while ((std::getline(input, line)) && isCorrect) {
        cameraPose = CameraPose();
        isCorrect = CreateCameraPose(cameraPose, line, headers);
        if (isCorrect) {
            cameraPoses.push_back(cameraPose);
        }
    }
    if (!isCorrect) {
        cameraPoses.clear();
    }

    input.close();

    return isCorrect;
}
