#ifndef COMPOSITE_CONTROLLER_H
#define COMPOSITE_CONTROLLER_H

#include "IController.h"
#include <vector>

class CompositeController : public IController
{
private:
	std::vector<IControllerPtr>	m_controllers;
	TerrainModel* _terrainModel;
	Mouse* m_mouse;
	Keyboard* m_keyboard;

public:
	CompositeController();

	virtual bool Initialize(TerrainModel* pModel, Mouse* mouse, Keyboard* keyboard) override;
	virtual bool CanHandle(ControllerEvent::IEvent* event) const override;
	virtual void Control(ControllerEvent::IEvent* event) override;
	virtual void SetTerrainModel(TerrainModel* pModel) override;
	virtual void SetMouse(Mouse* mouse) override;
	virtual void SetKeyboard(Keyboard* keyboard) override;
	virtual void Shutdown() override;

	void AddController(IControllerPtr controller);
	void RemoveController(IControllerPtr controller);

public:

};

#endif
