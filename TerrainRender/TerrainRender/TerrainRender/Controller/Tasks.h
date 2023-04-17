#ifndef CONTROLLER_TASKS_H
#define CONTROLLER_TASKS_H

#include "../Model/Persistence/ICallable.h"
#include "../Model/IModel.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <atomic>

class Task_LoadTerrainSoft : public ICallable
{
private:
	const std::wstring& m_filepath;
	IModelPtr m_modelPtr;
	std::atomic_bool& m_keepRunning;
	void Task();

public:
	Task_LoadTerrainSoft(const std::wstring& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning);
	virtual void operator()() override;
};

typedef std::shared_ptr<Task_LoadTerrainSoft> Task_LoadTerrainSoftPtr;
#endif // !CONTROLLER_TASKS_H

