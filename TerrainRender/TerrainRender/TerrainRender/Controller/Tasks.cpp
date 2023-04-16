#include "Tasks.h"

Task_LoadTerrainSoft::Task_LoadTerrainSoft(const std::wstring& filepath, IModelPtr modelPtr) : m_filepath(filepath), m_modelPtr(modelPtr) {}
void Task_LoadTerrainSoft::Task()
{
	m_modelPtr->LoadTerrainSoftEdges(m_filepath.c_str());
}
void Task_LoadTerrainSoft::operator()()
{
	Task();
 }