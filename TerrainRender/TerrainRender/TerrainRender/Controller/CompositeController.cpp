#include "CompositeController.h"
#include <algorithm>
CompositeController::CompositeController()
{
	_terrainModel = NULL;
	m_mouse = NULL;
	m_keyboard = NULL;
}

void CompositeController::SetTerrainModel(TerrainModel* pModel)
{
	this->_terrainModel = pModel;
}
void CompositeController::SetMouse(Mouse* mouse)
{
	this->m_mouse = mouse;
}
void CompositeController::SetKeyboard(Keyboard* keyboard)
{
	this->m_keyboard = keyboard;
}

bool CompositeController::Initialize(TerrainModel* pModel, Mouse* mouse, Keyboard* keyboard)
{
	if (pModel == nullptr || mouse == nullptr || keyboard == nullptr)
	{
		return false;
	}
	this->SetTerrainModel(pModel);
	this->SetMouse(mouse);
	this->SetKeyboard(keyboard);
	return true;
}
bool CompositeController::CanHandle(ControllerEvent::IEvent* event) const
{
	for (IControllerPtr controller : m_controllers)
	{
		if (controller->CanHandle(event))
			return true;
	}
	return false;
}
void CompositeController::Control(ControllerEvent::IEvent* event)
{
	for (IControllerPtr controller : m_controllers)
	{
		controller->Control(event);
	}
}

void CompositeController::Shutdown()
{
	for (IControllerPtr controller : m_controllers)
	{
		controller->Shutdown();
	}
}

void CompositeController::AddController(IControllerPtr controller)
{
	this->m_controllers.push_back(controller);
}
void CompositeController::RemoveController(IControllerPtr controller)
{
	auto it = std::find(m_controllers.begin(), m_controllers.end(), controller);
	if (it != m_controllers.end()) {
		m_controllers.erase(it);
	}
}
