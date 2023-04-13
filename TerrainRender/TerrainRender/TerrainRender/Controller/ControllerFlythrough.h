#ifndef CONTROLLER_FLYTHROUGH_H
#define CONTROLLER_FLYTHROUGH_H

#include "IController.h"
#include "MessageSystem.h"
#include "../Model/IModel.h"
#include "../View/IView.h"
#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"

class ControllerFlythrough : public IController
{
private:
	IModelPtr				m_terrainModel;
	MousePtr				m_mouse;
	KeyboardPtr				m_keyboard;
	std::vector<unsigned>	m_handledMsgs;
	ControllerMessageSystemPtr			m_messageSystem;

	bool					m_isActive = false;
	bool					m_isRunning = false;
	float					m_speed = 1.0f;


public:
	ControllerFlythrough();
	virtual ~ControllerFlythrough() {}

	virtual bool CanHandle(unsigned int message) const override;
	virtual void HandleMessage(unsigned int message, float* fparam, unsigned* uparam) override;
	virtual void SetTerrainModel(IModelPtr pModel) override;
	virtual void SetMouse(MousePtr mouse) override;
	virtual void SetKeyboard(KeyboardPtr keyboard) override;
	virtual void SetTerrainView(IViewPtr pView) override;


	virtual bool Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard) override;
	virtual bool IsActive() const  override;
	virtual void Shutdown() override;

private:

	void SetMessageSystem(ControllerMessageSystemPtr messageSystem);

};



#endif