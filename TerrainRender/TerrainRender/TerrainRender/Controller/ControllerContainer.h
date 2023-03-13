#ifndef CONTROLLER_CONTAINER_H
#define CONTROLLER_CONTAINER_H

#include "Controllers.h"
#include <vector>
class ControllerContainer
{
private:
	IController*				_guiController;
	std::vector<IController*>	_modeControllers;
	unsigned int				_currentModeControllerIndex;
public:

	ControllerContainer();
	bool FlipModeController();
	const IController* GetCurrentModeController();
	const IController* GetGuiController();
	void Shutdown();
};

#endif
