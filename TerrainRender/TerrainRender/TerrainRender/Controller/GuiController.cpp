#include "GuiController.h"
#include <algorithm>
#include "../Model/TerrainModel.h"
#include "ControllerEvents.h"

GuiController::GuiController()
{
	m_handledEvents.push_back(ControllerEvent::ClickButton_SetTerrainPath);
	m_handledEvents.push_back(ControllerEvent::ClickButton_SetTrajectoryPath);
	m_handledEvents.push_back(ControllerEvent::ClickButton_SetCameraPropertiesPath);
	m_handledEvents.push_back(ControllerEvent::EnterFloat_cameraMoveSpeed);
	m_handledEvents.push_back(ControllerEvent::EnterFloat_cameraRotationSpeed);
	m_handledEvents.push_back(ControllerEvent::ClickButton_ResetCamera);
}
GuiController::~GuiController() {}

bool GuiController::CanHandle(ControllerEvent::IEvent* event) const
{
	auto it = std::find(m_handledEvents.begin(), m_handledEvents.end(), event->GetEventType());
	return it != m_handledEvents.end();
}
void GuiController::Control(ControllerEvent::IEvent* event)
{
	if (event->GetEventType() == ControllerEvent::Type::ClickButton_SetTerrainPath)
	{
		if (ControllerEvent::TerrainFileSelectEvent* castedEvent = dynamic_cast<ControllerEvent::TerrainFileSelectEvent*>(event)) 
		{
			const wchar_t* filepath = castedEvent->GetFilePath();
			this->m_terrainModel->LoadTerrain(filepath);
		}
	}
	if (event->GetEventType() == ControllerEvent::Type::EnterFloat_cameraMoveSpeed)
	{
		if (ControllerEvent::EnterFloatEvent* castedEvent = dynamic_cast<ControllerEvent::EnterFloatEvent*>(event))
		{
			float speed = castedEvent->GetFloatData();
			ModelEvent::SetCameraMoveSpeedEvent event;
			event.SetData(speed);
			this->m_terrainModel->EventHandler(&event);
		}
	}

	if (event->GetEventType() == ControllerEvent::Type::EnterFloat_cameraRotationSpeed)
	{
		if (ControllerEvent::EnterFloatEvent* castedEvent = dynamic_cast<ControllerEvent::EnterFloatEvent*>(event))
		{
			float speed = castedEvent->GetFloatData();
			ModelEvent::SetCameraRotationSpeedEvent event;
			event.SetData(speed);
			this->m_terrainModel->EventHandler(&event);
		}
	}

	if (event->GetEventType() == ControllerEvent::Type::ClickButton_ResetCamera)
	{
		if (ControllerEvent::Event* castedEvent = dynamic_cast<ControllerEvent::Event*>(event))
		{
			ModelEvent::Event event;
			event.SetEventType(ModelEvent::ResetCamera);
			this->m_terrainModel->EventHandler(&event);
		}
	}
}
void GuiController::SetTerrainModel(TerrainModel* pModel)
{
	this->m_terrainModel = pModel;
}
void GuiController::SetMouse(Mouse* mouse) {}

void GuiController::SetKeyboard(Keyboard* keyboard) {}


bool GuiController::Initialize(TerrainModel* pModel, Mouse* mouse, Keyboard* keyboard) 
{
	if (pModel == nullptr)
		return false;
	SetTerrainModel(pModel);
	return true;
}
void GuiController::Shutdown() {}