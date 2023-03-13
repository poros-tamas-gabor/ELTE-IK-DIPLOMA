#include "GuiController.h"
#include "../Model/ModelLayer.h"

GuiController::GuiController(ModelLayer* model) : _model(model) {}

void GuiController::OnFileLoaded(const FileSelectEvent& event) const
{
	if (event.IsTerrainFile())
	{
		this->_model->LoadTerrain(event.GetFilePath().c_str());
	}
}