#ifndef ICALLABLE_H
#define ICALLABLE_H
#include <memory>
#include <string>

class ICallable
{
public:
	virtual ~ICallable() = default;
	virtual void operator()() = 0;
};

typedef std::shared_ptr<ICallable> ICallablePtr;

class ICallableCreator
{
public :
	virtual ~ICallableCreator() = default;
	virtual ICallablePtr Create(void) const = 0;
};

#endif