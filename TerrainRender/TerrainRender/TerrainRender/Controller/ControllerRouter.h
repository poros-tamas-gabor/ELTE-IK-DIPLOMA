#ifndef CONTROLLER_ROUTER_H
#define CONTROLLER_ROUTER_H

///////////////////////////////////////////////////////////////////////////////
// ControllerRouter.h
// ==================
// A derived class of the IController interface that serves as the main controller
// for the application. This class is responsible for delegating tasks to the other
// controllers, based on the current mode of the application.
//
// To use this class, it must be instantiated and registered with the View and Model components of the application.
// Once registered, it will receive user input from the View and delegate commands to the other controllers.
//
// The ControllerRouter maintains references to the other controllers and sends messages to each of them via the ControllerMessageSystem.
// The ControllerRouter is also responsible for adding the other controllers as subscribers to the MessageSystem via the AddController method.
// It is up to the other Controllers to decide what to do with the messages.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
//////////////////////////////////////////////////////////////////////////

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
	ControllerMessageSystemPtr		m_messageSystem;
	std::vector<IControllerPtr>		m_controllers;
	IModelPtr						m_terrainModel;
	IViewPtr						m_terrainView;
	MousePtr						m_mouse;
	KeyboardPtr						m_keyboard;
	bool							m_isActive = true;

public:
	ControllerRouter();
	ControllerRouter operator=(const ControllerRouter&) = delete;
	ControllerRouter(const ControllerRouter&) = delete;
	virtual ~ControllerRouter() {}

	virtual bool Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard) override;
	virtual bool HandleMessage(IControllerMessageIDs message, const std::vector<float>& fparam, const std::vector<unsigned>& uparam) override;
	virtual bool IsActive() const  override;
	virtual void Shutdown() override;

	virtual void SetTerrainModel(IModelPtr pModel) override;
	virtual void SetMouse(MousePtr mouse) override;
	virtual void SetKeyboard(KeyboardPtr keyboard) override;
	virtual void SetTerrainView(IViewPtr pView) override;

	void HandleIModelState(const MeshGroupState&) override;
	void HandleIModelState(const FlythroughState&) override;
	void HandleIModelState(const Explore3DState&) override;
	void HandleIModelState(const GeneralModelState&) override;

	bool AddController(IControllerPtr controller);
	bool RemoveController(IControllerPtr controller);

private:
	void SetMessageSystem(ControllerMessageSystemPtr messageSystem);
};

typedef std::shared_ptr<ControllerRouter> CompositeControllerPtr;

#endif
