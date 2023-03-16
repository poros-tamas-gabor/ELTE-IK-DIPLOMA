#include "ControllerContainer.h"


ControllerContainer::ControllerContainer() : _guiController(nullptr), _currentModeControllerIndex(-1), _keyboard(nullptr), _mouse(nullptr), _model(nullptr), _view(nullptr)
{

}

bool ControllerContainer::Initalize(Keyboard& keyboard, Mouse& mouse)
{
	_currentModeControllerIndex = 0;
	_keyboard	= &keyboard;
	_mouse		= &mouse;

	IController* controller = new Controller3DExplore(this, _model, _view);
	if (controller == nullptr)
	{
		return false;
	}
	_modeControllers.push_back(controller);

	_guiController = new GuiController(_model);
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
const GuiController* ControllerContainer::GetGuiController()
{
	return this->_guiController;
}

void ControllerContainer::SetModel(ModelLayer* model)
{
	this->_model = model;
}
void ControllerContainer::SetView(TerrainModel* view)
{
	this->_view = view;
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