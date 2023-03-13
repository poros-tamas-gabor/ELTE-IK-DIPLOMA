#include "ControllerContainer.h"



ControllerContainer::ControllerContainer() : _guiController(nullptr)
{
	_currentModeControllerIndex = 0;
	_modeControllers.push_back(new Controller3DExplore);
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