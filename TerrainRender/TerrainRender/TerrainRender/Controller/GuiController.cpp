#include "GuiController.h"
#include <algorithm>
#include "../Model/TerrainModel.h"
#include "ControllerEvents.h"

GuiController::GuiController()
{
	m_handledEvents.push_back(ControllerEvent::ClickButton_SetTerrainPath);
	m_handledEvents.push_back(ControllerEvent::ClickButton_SetTrajectoryPath);
	m_handledEvents.push_back(ControllerEvent::ClickButton_SetCameraPropertiesPath);
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