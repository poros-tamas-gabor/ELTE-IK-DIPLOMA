#ifndef ICALLABLE_H
#define ICALLABLE_H

///////////////////////////////////////////////////////////////////////////////
// ICallable.h
// ===============
// This header defines two interface called ICallable and ICallableCreator.
//
// The idea behind ICallable interface is to provide a generic way of creating callable objects 
// that can be executed in a separate thread using the std::thread class. By creating a class that derives from ICallable 
// and implements the operator() function, you can create a callable object that can be passed to a std::thread object 
// to be executed asynchronously.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include <memory>

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