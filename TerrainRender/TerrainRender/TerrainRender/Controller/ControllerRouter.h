#ifndef ROUTER_CONTROLLER_H
#define ROUTER_CONTROLLER_H

#include "IController.h"
#include "MessageSystem.h"
#include "../Model/IModel.h"
#include "../View/IView.h"
#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"
#include <vector>
class ControllerRouter : public IController
{
private:
	ControllerMessageSystemPtr	m_messageSystem;
	std::vector<IControllerPtr>	m_controllers;
	IModelPtr					m_terrainModel;
	IViewPtr					m_terrainView;
	MousePtr					m_mouse;
	KeyboardPtr					m_keyboard;
	bool						m_isActive;

public:
	ControllerRouter();

	virtual bool CanHandle(unsigned int message) const override;
	virtual void HandleMessage(unsigned int message, float* fparam, unsigned* uparam) override;
	virtual void SetTerrainModel(IModelPtr pModel) override;
	virtual void SetMouse(MousePtr mouse) override;
	virtual void SetKeyboard(KeyboardPtr keyboard) override;
	virtual void SetTerrainView(IViewPtr pView) override;
	virtual bool IsFlythroughModeOn(void) const override;


	virtual bool Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard) override;
	virtual bool IsActive() const  override;
	virtual void Shutdown() override;

	void AddController(IControllerPtr controller);
	void RemoveController(IControllerPtr controller);

private:
	void SetMessageSystem(ControllerMessageSystemPtr messageSystem);

};

typedef std::shared_ptr<ControllerRouter> CompositeControllerPtr;

#endif
