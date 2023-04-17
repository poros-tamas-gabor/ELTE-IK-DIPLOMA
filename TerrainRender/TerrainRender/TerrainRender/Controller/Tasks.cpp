#include "Tasks.h"


#include <CommCtrl.h>
#include "../ErrorHandler.h"
#include "../resource.h"
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


Task_LoadTerrainSoft::Task_LoadTerrainSoft(const std::wstring& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning) : m_filepath(filepath), m_modelPtr(modelPtr), m_keepRunning(keepRunning) {}
void Task_LoadTerrainSoft::Task()
{
	m_modelPtr->LoadTerrainSoftEdges(m_filepath.c_str());
    m_keepRunning = false;
}
void Task_LoadTerrainSoft::operator()()
{
	Task();
 }


