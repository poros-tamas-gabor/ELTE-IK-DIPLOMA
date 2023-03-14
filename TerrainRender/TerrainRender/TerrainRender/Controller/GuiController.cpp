#include "GuiController.h"
#include "../Model/ModelLayer.h"

GuiController::GuiController(ModelLayer* model) : _model(model) {}

void GuiController::OnFileLoaded(const FileSelectEvent::Event& event) const
{
	if (event.IsType(FileSelectEvent::Type::TerrainFile))
	{
		this->_model->LoadTerrain(event.GetData().c_str());
	}
	else if (event.IsType(FileSelectEvent::Type::CameraTrajectoryFile))
	{
		this->_model->LoadCameraTrajectory(event.GetData().c_str());
	}
}