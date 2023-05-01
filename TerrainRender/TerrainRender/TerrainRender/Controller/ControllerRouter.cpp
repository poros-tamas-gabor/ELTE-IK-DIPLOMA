#include "ControllerRouter.h"
#include "ControllerFlythrough.h"
#include <algorithm>
ControllerRouter::ControllerRouter() : m_isActive(true)
{
	m_messageSystem = std::make_shared<ControllerMessageSystem>(m_controllers);
	m_terrainModel	= nullptr;
	m_mouse			= nullptr;
	m_keyboard		= nullptr;
}

void ControllerRouter::SetMessageSystem(ControllerMessageSystemPtr) {}

void ControllerRouter::SetTerrainView(IViewPtr pView)
{
	this->m_terrainView = pView;
}
void ControllerRouter::SetTerrainModel(IModelPtr pModel)
{
	this->m_terrainModel = pModel;
}
void ControllerRouter::SetMouse(MousePtr mouse)
{
	this->m_mouse = mouse;
}
void ControllerRouter::SetKeyboard(KeyboardPtr keyboard)
{
	this->m_keyboard = keyboard;
}

bool ControllerRouter::Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard)
{
	if (pModel == nullptr || mouse == nullptr || keyboard == nullptr || pView == nullptr)
	{
		return false;
	}
	this->SetTerrainModel(pModel);
	this->SetTerrainView(pView);
	this->SetMouse(mouse);
	this->SetKeyboard(keyboard);
	return true;
}


bool ControllerRouter::HandleMessage(IControllerMessageIDs message, const std::vector<float>& fparams, const std::vector<unsigned>& uparams)
{
	return m_messageSystem->Publish(message, fparams, uparams);
}

void ControllerRouter::Shutdown()
{
	for (IControllerPtr controller : m_controllers)
	{
		controller->Shutdown();
	}
}

bool ControllerRouter::IsActive() const
{
	return this->m_isActive;
}

bool ControllerRouter::AddController(IControllerPtr controller)
{
	if (controller == nullptr)
		return false;
	if (std::find(this->m_controllers.begin(), this->m_controllers.end(), controller) != this->m_controllers.end())
	{
		return false;
	}
	else {
		controller->SetMessageSystem(m_messageSystem);
		this->m_controllers.push_back(controller);
		return true;
	}
}
bool ControllerRouter::RemoveController(IControllerPtr controller)
{
	if (controller == nullptr)
		return false;
	auto it = std::find(m_controllers.begin(), m_controllers.end(), controller);
	if (it != m_controllers.end()) {
		m_controllers.erase(it);
		return true;
	}
	return false;
}

void ControllerRouter::HandleIModelState(const MeshGroupState& states)
{
	for (IControllerPtr controller : m_controllers)
	{
		controller->HandleIModelState(states);
	}
}
void ControllerRouter::HandleIModelState(const FlythroughState& state)
{
	for (IControllerPtr controller : m_controllers)
	{
		controller->HandleIModelState(state);
	}
}
void ControllerRouter::HandleIModelState(const Explore3DState& state)
{
	for (IControllerPtr controller : m_controllers)
	{
		controller->HandleIModelState(state);
	}
}
void ControllerRouter::HandleIModelState(const GeneralModelState& state)
{
	for (IControllerPtr controller : m_controllers)
	{
		controller->HandleIModelState(state);
	}
}
