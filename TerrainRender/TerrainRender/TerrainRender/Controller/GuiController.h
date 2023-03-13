#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include "FileSelectEvent.h"
#include <string>
class ModelLayer;
class GuiController
{
public:
	GuiController(ModelLayer* model);
	void OnFileLoaded(const FileSelectEvent& event) const;
private:
	ModelLayer*	_model;
};

#endif