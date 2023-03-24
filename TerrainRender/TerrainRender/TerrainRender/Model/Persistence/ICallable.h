#ifndef ICALLABLE_H
#define ICALLABLE_H
#include <memory>
#include <string>

class TextFileDataAccessAsync;
class ICallable
{
public:
	virtual ~ICallable() = default;
	virtual void operator()() = 0;
};

typedef std::shared_ptr<ICallable> ICallablePtr;

class ICallableCreator
{
public:
	virtual ~ICallableCreator() = default;
	virtual void Initialize(const std::wstring& filepath, int start, int end, TextFileDataAccessAsync* persistence) = 0;
	virtual ICallablePtr Create() = 0;
};

typedef std::shared_ptr<ICallableCreator> ICallableCreatorPtr;

#endif