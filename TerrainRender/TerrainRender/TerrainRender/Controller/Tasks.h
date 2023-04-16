#ifndef CONTROLLER_TASKS_H
#define CONTROLLER_TASKS_H

#include "../Model/Persistence/ICallable.h"
#include "../Model/IModel.h"


class Task_LoadTerrainSoft : public ICallable
{
private:
	const std::wstring& m_filepath;
	IModelPtr m_modelPtr;

public:
	Task_LoadTerrainSoft(const std::wstring& filepath, IModelPtr modelPtr);
	void Task();
	virtual void operator()() override;
};

typedef std::shared_ptr<Task_LoadTerrainSoft> Task_LoadTerrainSoftPtr;
#endif // !CONTROLLER_TASKS_H

