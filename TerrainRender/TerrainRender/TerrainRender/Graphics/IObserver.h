#ifndef I_OBSERVER_H
#define I_OBSERVER_H
#include "../Model/ModelEvent.h"

class IObserver
{
public:
	virtual ~IObserver() {}
	virtual void Update(const ModelEvent::Event& event) = 0;
};
#endif