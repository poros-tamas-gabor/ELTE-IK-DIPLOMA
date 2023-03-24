#include "DataAccess.h"

#include <fstream>
#include <string>
#include <windows.h>
#include "ModelStructs.h"
#include <vector>
#include <regex>

bool TextFileDataAccess::CreateVertex(Vertex& vertex, const std::string& line, std::vector<Vertex>& vertices, STLLineType& type, int& vertexCount) {
    bool bresult;
    switch (type) {
    case BEGIN: {
        type = FACET;
        return true;
    }
    case FACET:
    case END: {
        vertex.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        vertex.normal = { 0,0,0 };
        vertex.position = { 0,0,0 };

        if (line.find("endsolid") != std::string::npos) {
            return true;
        }

        std::istringstream iss(line);
        std::string word;
        iss >> word >> word; // skip "facet" and "normal"
        iss >> vertex.normal.x >> vertex.normal.y >> vertex.normal.z;
        type = LOOP;
        return true;
    }
    case LOOP: {
        type = VERTEX;
        return true;
    }
    case VERTEX: {
        std::istringstream iss(line);
        std::string word;
        iss >> word; // skip "vertex"
        iss >> vertex.position.x >> vertex.position.y >> vertex.position.z;
        vertices.push_back(vertex);

        if (vertexCount < 2) {
            vertexCount++;
            type = VERTEX;
        }
        else  {
            vertexCount = 0;
            type = ENDLOOP;
        }
        return true;
    }
    case ENDLOOP: {
        type = ENDFACET;
        return true;
    }
    case ENDFACET: {
        type = FACET;
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

    std::istringstream lineStream(line);
    std::string word;

    int i = -1;
    while ((std::getline(lineStream, word, ';'))) {
        i++;
        if (headers.at(i) == "sec") {
            cameraPose.epochtime.setSeconds(std::stoll(word));
        }
        else if (headers.at(i) == "nsec") {
            cameraPose.epochtime.setNanoseconds(std::stoll(word));
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
