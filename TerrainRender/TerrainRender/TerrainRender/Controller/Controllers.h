#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"

class Graphics;
class IController
{
protected:
	Keyboard& _keyboard;
	Mouse& _mouse;
public:
	IController(Keyboard& keyboard, Mouse& mouse) : _keyboard(keyboard), _mouse(mouse) {}
	virtual ~IController() {}
	virtual void Control(float dt, Graphics* graphics)const = 0;
};


class Controller3DExplore : public IController
{
public:
	Controller3DExplore(Keyboard& keyboard, Mouse& mouse) : IController(keyboard, mouse) {}
	virtual ~Controller3DExplore() {}
	virtual void Control(float dt, Graphics* graphics) const override;

private:

	void ControlMouse(const MouseEvent& e, Graphics* graphics) const;
	void ControlKeyboard(const KeyboardEvent& e, float dt, Graphics* graphics) ;

};

class ControllerFlythrough : public IController
{
public:
	ControllerFlythrough(Keyboard& keyboard, Mouse& mouse) : IController(keyboard, mouse) {}
	virtual ~ControllerFlythrough() {}
	virtual void Control(float dt, Graphics* graphics) const override;
};


#endif