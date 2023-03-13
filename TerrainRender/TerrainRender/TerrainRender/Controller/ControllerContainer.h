#ifndef CONTROLLER_CONTAINER_H
#define CONTROLLER_CONTAINER_H

#include "Controllers.h"
#include "GuiController.h"
#include <vector>
#include "../Graphics/Graphics.h"
#include "../Model/ModelLayer.h"


class ControllerContainer
{
private:
	GuiController*				_guiController;
	std::vector<IController*>	_modeControllers;
	int							_currentModeControllerIndex;
	ModelLayer*					_model;
	Graphics*					_view;

public:
	Keyboard*					_keyboard;
	Mouse*						_mouse;


public:
	ControllerContainer();

	bool Initalize(Keyboard& keyboard, Mouse& mouse);
	void SetModel(ModelLayer* model);
	void SetView(Graphics* view);
	bool FlipModeController();
	const IController* GetCurrentModeController();
	const GuiController* GetGuiController();
	void Shutdown();
};

#endif
