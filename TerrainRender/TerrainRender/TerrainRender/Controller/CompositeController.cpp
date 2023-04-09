#include "CompositeController.h"
#include <algorithm>
CompositeController::CompositeController() : m_isActive(true)
{
	m_messageSystem = std::make_shared<ControllerMessageSystem>(m_controllers);
	m_terrainModel = NULL;
	m_mouse = NULL;
	m_keyboard = NULL;
}


void CompositeController::SetMessageSystem(ControllerMessageSystemPtr) {}



void CompositeController::SetTerrainModel(IModelPtr pModel)
{
	this->m_terrainModel = pModel;
}
void CompositeController::SetMouse(MousePtr mouse)
{
	this->m_mouse = mouse;
}
void CompositeController::SetKeyboard(KeyboardPtr keyboard)
{
	this->m_keyboard = keyboard;
}

bool CompositeController::Initialize(IModelPtr pModel, MousePtr mouse, KeyboardPtr keyboard)
{
	if (pModel.get() == nullptr || mouse.get() == nullptr || keyboard.get() == nullptr )
	{
		return false;
	}
	this->SetTerrainModel(pModel);
	this->SetMouse(mouse);
	this->SetKeyboard(keyboard);
	return true;
}

bool CompositeController::CanHandle(unsigned int message) const
{
	for (IControllerPtr controller : m_controllers)
	{
		if (controller->CanHandle(message))
			return true;
	}
	return false;
}

void CompositeController::HandleMessage(unsigned int message, float* fparam, unsigned* uparam)
{
	m_messageSystem->Publish(message, fparam, uparam);
}

void CompositeController::Shutdown()
{
	for (IControllerPtr controller : m_controllers)
	{
		controller->Shutdown();
	}
}

bool CompositeController::IsActive() const
{
	return this->m_isActive;
}

void CompositeController::AddController(IControllerPtr controller)
{
	controller->SetMessageSystem(m_messageSystem);
	this->m_controllers.push_back(controller);
}
void CompositeController::RemoveController(IControllerPtr controller)
{
	auto it = std::find(m_controllers.begin(), m_controllers.end(), controller);
	if (it != m_controllers.end()) {
		m_controllers.erase(it);
	}
}
