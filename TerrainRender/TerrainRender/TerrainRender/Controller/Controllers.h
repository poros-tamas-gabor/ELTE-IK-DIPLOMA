#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"

class Graphics;
class IController
{
public:
	virtual ~IController() {}
	virtual void Control(float dt, Graphics* graphics)const = 0;
};


class Controller3DExplore : public IController
{
public:
	virtual ~Controller3DExplore() {}
	virtual void Control(float dt, Graphics* graphics) const override;
private:
	void ControlMouse(const MouseEvent& e, Graphics* graphics) const;
	void ControlKeyboard(const KeyboardEvent& e, float dt, Graphics* graphics) ;

};

class ControllerFlythrough : public IController
{
public:
	virtual ~ControllerFlythrough() {}
	virtual void Control(float dt, Graphics* graphics) const override;
};


#endif