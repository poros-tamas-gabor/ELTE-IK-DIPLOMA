#ifndef ICALLABLE_H
#define ICALLABLE_H
#include <memory>
#include <string>

class BinaryFileDataAccessAsync;
class ICallable
{
public:
	virtual ~ICallable() = default;
	virtual void operator()() = 0;
};

typedef std::shared_ptr<ICallable> ICallablePtr;


#endif