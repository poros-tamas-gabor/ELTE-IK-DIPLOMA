#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"

class Graphics;
class ControllerContainer;
class ModelLayer;
class IController
{

public:
	virtual ~IController() {}
	virtual void Control(float dt)const = 0;
};


class Controller3DExplore : public IController
{
private:
	ControllerContainer* _container;
	ModelLayer*			_model;
	Graphics*			_graphics;
public:
	Controller3DExplore(ControllerContainer* _container, ModelLayer* model, Graphics* graphics);
	virtual ~Controller3DExplore() {}
	virtual void Control(float dt) const override;



private:

	void ControlMouse(const MouseEvent& e) const;
	void ControlKeyboard(const KeyboardEvent& e, float dt) ;

};

class ControllerFlythrough : public IController
{
public:
	ControllerFlythrough() {}
	virtual ~ControllerFlythrough() {}
	virtual void Control(float dt) const override;
};


#endif