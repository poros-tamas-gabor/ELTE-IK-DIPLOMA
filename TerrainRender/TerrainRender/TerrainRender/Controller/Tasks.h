#ifndef CONTROLLER_TASKS_H
#define CONTROLLER_TASKS_H

#include "../Model/Persistence/ICallable.h"
#include "../Model/IModel.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <atomic>

class Task_LoadTerrain_Soft : public ICallable
{
private:
	const std::wstring& m_filepath;
	IModelPtr m_modelPtr;
	std::atomic_bool& m_keepRunning;
	void Task();

public:
	Task_LoadTerrain_Soft(const std::wstring& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning);
	virtual void operator()() override;
};

class Creator_LoadTerrain_Soft : public ICallableCreator
{
private:
	const std::wstring& m_filepath;
	IModelPtr m_modelPtr;
	std::atomic_bool& m_keepRunning;
public:
	Creator_LoadTerrain_Soft(const std::wstring& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning);
	ICallablePtr Create() const override;
};

class Task_LoadProject_Soft : public ICallable
{
private:
	const std::vector<std::wstring>& m_filepaths;
	IModelPtr m_modelPtr;
	std::atomic_bool& m_keepRunning;
	void Task();

public:
	Task_LoadProject_Soft(const std::vector<std::wstring>& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning);
	virtual void operator()() override;
};

class Creator_LoadProject_Soft : public ICallableCreator
{
private:
	const std::vector<std::wstring>& m_filepaths;
	IModelPtr m_modelPtr;
	std::atomic_bool& m_keepRunning;
public:
	Creator_LoadProject_Soft(const std::vector<std::wstring>& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning);
	ICallablePtr Create() const override;
};

class Task_LoadTerrain_Sharp : public ICallable
{
private:
	const std::wstring& m_filepath;
	IModelPtr m_modelPtr;
	std::atomic_bool& m_keepRunning;
	void Task();

public:
	Task_LoadTerrain_Sharp(const std::wstring& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning);
	virtual void operator()() override;
};

class Creator_LoadTerrain_Sharp : public ICallableCreator
{
private:
	const std::wstring& m_filepath;
	IModelPtr m_modelPtr;
	std::atomic_bool& m_keepRunning;
public:
	Creator_LoadTerrain_Sharp(const std::wstring& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning);
	ICallablePtr Create() const override;
};

class Task_LoadProject_Sharp : public ICallable
{
private:
	const std::vector<std::wstring>& m_filepaths;
	IModelPtr m_modelPtr;
	std::atomic_bool& m_keepRunning;
	void Task();

public:
	Task_LoadProject_Sharp(const std::vector<std::wstring>& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning);
	virtual void operator()() override;
};

class Creator_LoadProject_Sharp : public ICallableCreator
{
private:
	const std::vector<std::wstring>& m_filepaths;
	IModelPtr m_modelPtr;
	std::atomic_bool& m_keepRunning;
public:
	Creator_LoadProject_Sharp(const std::vector<std::wstring>& filepath, IModelPtr modelPtr, std::atomic_bool& keepRunning);
	ICallablePtr Create() const override;
};

#endif // !CONTROLLER_TASKS_H

