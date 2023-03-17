#ifndef COMPOSITE_CONTROLLER_H
#define COMPOSITE_CONTROLLER_H

#include "IController.h"
#include <vector>

class CompositeController : public IController
{
private:
	std::vector<IControllerPtr>	m_controllers;
	TerrainModel* m_terrainModel;
	Mouse* m_mouse;
	Keyboard* m_keyboard;

public:
	CompositeController();

	virtual bool Initialize(TerrainModel* pModel, Mouse* mouse, Keyboard* keyboard) override;
	virtual bool CanHandle(unsigned int message) const override;
	virtual void Control(unsigned int message, float* fparam, unsigned* uparam) override;
	virtual void SetTerrainModel(TerrainModel* pModel) override;
	virtual void SetMouse(Mouse* mouse) override;
	virtual void SetKeyboard(Keyboard* keyboard) override;
	virtual void Shutdown() override;

	void AddController(IControllerPtr controller);
	void RemoveController(IControllerPtr controller);

public:

};

#endif
