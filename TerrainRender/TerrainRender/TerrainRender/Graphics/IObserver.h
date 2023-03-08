#ifndef I_OBSERVER_H
#define I_OBSERVER_H


class IObserver
{
public:
	virtual ~IObserver() {}
	virtual void Update() = 0;
};
#endif