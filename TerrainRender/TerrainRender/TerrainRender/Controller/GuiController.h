#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include "IController.h"
class GuiController : public IController
{
private:
	TerrainModel*							m_terrainModel;
	std::vector<ControllerEvent::Type>		m_handledEvents;

public:
	GuiController();
	virtual ~GuiController();

	virtual bool CanHandle(ControllerEvent::IEvent* event) const override;
	virtual void Control(ControllerEvent::IEvent* event) override;
	virtual void SetTerrainModel(TerrainModel* pModel) override;
	virtual void SetMouse(Mouse* mouse) override;
	virtual void SetKeyboard(Keyboard* keyboard) override;

	virtual bool Initialize(TerrainModel* pModel, Mouse* mouse, Keyboard* keyboard) override;
	virtual void Shutdown() override;

private:
};



#endif