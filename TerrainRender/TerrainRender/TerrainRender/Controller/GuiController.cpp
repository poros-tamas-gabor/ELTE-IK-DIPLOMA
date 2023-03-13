#include "GuiController.h"
#include "../Model/ModelLayer.h"

GuiController::GuiController(ModelLayer* model) : _model(model) {}

void GuiController::OnFileLoaded(const FileSelectEvent& event) const
{
	if (event.IsType(FileSelectEvent_Type::TerrainFile))
	{
		this->_model->LoadTerrain(event.GetData().c_str());
	}
}