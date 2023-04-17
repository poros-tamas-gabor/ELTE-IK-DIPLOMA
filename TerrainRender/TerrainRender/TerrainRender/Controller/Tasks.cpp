#include "Tasks.h"
#include <memory>

//Load Terrain Soft Edges
Task_LoadTerrain_Soft::Task_LoadTerrain_Soft(const std::wstring& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning) 
	: m_filepath(filepath), m_modelPtr(modelPtr), m_keepRunning(keepRunning) {}

void Task_LoadTerrain_Soft::Task()
{
	m_keepRunning = true;
	m_modelPtr->LoadTerrainSoftEdges(m_filepath.c_str());
    m_keepRunning = false;
}
void Task_LoadTerrain_Soft::operator()()
{
	Task();
}

Creator_LoadTerrain_Soft::Creator_LoadTerrain_Soft(const std::wstring& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning) 
	: m_filepath(filepath), m_modelPtr(modelPtr), m_keepRunning(keepRunning) {}

ICallablePtr Creator_LoadTerrain_Soft::Create() const
{
	return std::make_shared<Task_LoadTerrain_Soft>(m_filepath, m_modelPtr, m_keepRunning);
}


//Load Terrain Sharp Edges
Task_LoadTerrain_Sharp::Task_LoadTerrain_Sharp(const std::wstring& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning) 
	: m_filepath(filepath), m_modelPtr(modelPtr), m_keepRunning(keepRunning) {}
void Task_LoadTerrain_Sharp::Task()
{
	m_keepRunning = true;
	m_modelPtr->LoadTerrainSharpEdges(m_filepath.c_str());
	m_keepRunning = false;
}
void Task_LoadTerrain_Sharp::operator()()
{
	Task();
}

Creator_LoadTerrain_Sharp::Creator_LoadTerrain_Sharp(const std::wstring& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning)
	: m_filepath(filepath), m_modelPtr(modelPtr), m_keepRunning(keepRunning) {}

ICallablePtr Creator_LoadTerrain_Sharp::Create() const
{
	return std::make_shared<Task_LoadTerrain_Sharp>(m_filepath, m_modelPtr, m_keepRunning);
}


//Load Project Soft Edges
Task_LoadProject_Soft::Task_LoadProject_Soft(const std::vector<std::wstring>& filepaths, IModelPtr modelPtr, std::atomic_bool& keepRunning)
	: m_filepaths(filepaths), m_modelPtr(modelPtr), m_keepRunning(keepRunning) {}

void Task_LoadProject_Soft::Task()
{
	m_modelPtr->LoadTerrainSoftEdges_Project(m_filepaths);
	m_keepRunning = false;
}
void Task_LoadProject_Soft::operator()()
{
	Task();
}

Creator_LoadProject_Soft::Creator_LoadProject_Soft(const std::vector<std::wstring>& filepaths, IModelPtr modelPtr, std::atomic_bool& keepRunning)
	: m_filepaths(filepaths), m_modelPtr(modelPtr), m_keepRunning(keepRunning) {}

ICallablePtr Creator_LoadProject_Soft::Create() const
{
	return std::make_shared<Task_LoadProject_Soft>(m_filepaths, m_modelPtr, m_keepRunning);
}

//Load Project Sharp Edges

Task_LoadProject_Sharp::Task_LoadProject_Sharp(const std::vector<std::wstring>& filepaths, IModelPtr modelPtr, std::atomic_bool& keepRunning)
	: m_filepaths(filepaths), m_modelPtr(modelPtr), m_keepRunning(keepRunning) {}

void Task_LoadProject_Sharp::Task()
{
	m_modelPtr->LoadTerrainSharpEdges_Project(m_filepaths);
	m_keepRunning = false;
}
void Task_LoadProject_Sharp::operator()()
{
	Task();
}

Creator_LoadProject_Sharp::Creator_LoadProject_Sharp(const std::vector<std::wstring>& filepaths, IModelPtr modelPtr, std::atomic_bool& keepRunning)
	: m_filepaths(filepaths), m_modelPtr(modelPtr), m_keepRunning(keepRunning) {}

ICallablePtr Creator_LoadProject_Sharp::Create() const
{
	return std::make_shared<Task_LoadProject_Sharp>(m_filepaths, m_modelPtr, m_keepRunning);
}
