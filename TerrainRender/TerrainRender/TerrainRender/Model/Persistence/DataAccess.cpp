#include "DataAccess.h"

#include <fstream>
#include <string>
#include <windows.h>
#include "ModelStructs.h"
#include <vector>
#include <regex>
#include "../../ErrorHandler.h"

bool TextFileDataAccess::CreateVertex(VertexMesh& vertex, const std::string& line, std::vector<VertexMesh>& vertices, STLLineType& type, int& vertexCount) {
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
        if (iss.fail())
        {
            THROW_TREXCEPTION(L"failed to load facet normal");
        }
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
        if (iss.fail())
        {
            THROW_TREXCEPTION(L"failed to load facet position");
        }
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

bool TextFileDataAccess::LoadTerrain(const wchar_t* filename, std::vector<VertexMesh>& vertices)
{
	std::ifstream   input;
	std::string     line;
    STLLineType     type = STLLineType::BEGIN;
	VertexMesh          vertex;

	input.open(filename, std::ifstream::in);

	if (!input.is_open())
		return false;
    try
    {
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
    catch (TRException& e)
    {
        ErrorHandler::Log(e);
    }
    catch (std::exception& e)
    {
        ErrorHandler::Log(e);
    }
    catch (...)
    {
        ErrorHandler::Log("Unknown exception");
    }
    return false;
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


    try
    {
        bool isCorrect = true;
        // read first line
        std::getline(input, line);

        //read the headers
        std::istringstream lineStream(line);
        while ((std::getline(lineStream, word, ';'))) {
            headers.push_back(word);
        }

        std::vector<std::string>::iterator it;
        //TODO: CHECK headers
        for (it = headers.begin(); it != headers.end(); it++)
        {
            if(*it == "nsec")
                break;
        }
        if (it == headers.end())
        {
            THROW_TREXCEPTION(L"nsec missing");
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
    catch (TRException& e)
    {
        ErrorHandler::Log(e);
    }
    catch (std::exception& e)
    {
        ErrorHandler::Log(e);
    }
    catch (...)
    {
        ErrorHandler::Log("Unknown exception");
    }
    return false;

}
