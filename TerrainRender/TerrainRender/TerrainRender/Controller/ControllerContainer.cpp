#include "ControllerContainer.h"



ControllerContainer::ControllerContainer() : _guiController(nullptr), _currentModeControllerIndex(-1), _keyboard(nullptr), _mouse(nullptr)
{

}

bool ControllerContainer::Initalize(Keyboard& keyboard, Mouse& mouse)
{
	_currentModeControllerIndex = 0;
	_keyboard	= &keyboard;
	_mouse		= &mouse;
	IController* controller = new Controller3DExplore(*_keyboard, *_mouse);
	if (controller == nullptr)
	{
		return false;
	}
	_modeControllers.push_back(controller);
	return true;
}

bool ControllerContainer::FlipModeController()
{
	return false;
}

;
const IController* ControllerContainer::GetCurrentModeController()
{
	return _modeControllers.at(_currentModeControllerIndex);
}
const IController* ControllerContainer::GetGuiController()
{
	return _guiController;
}

void ControllerContainer::Shutdown()
{
	for (IController* c : this->_modeControllers)
	{
		if (c != nullptr)
		{
			delete c;
			c = nullptr;
		}
	}
	if (this->_guiController != nullptr)
	{
		delete this->_guiController;
		this->_guiController = nullptr;
	}
}