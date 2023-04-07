#ifndef COMPOSITE_CONTROLLER_H
#define COMPOSITE_CONTROLLER_H

#include "IController.h"
#include "MessageSystem.h"
#include <vector>
class CompositeController : public IController
{
private:
	MessageSystem				m_messageSystem;
	std::vector<IControllerPtr>	m_controllers;
	IModelPtr					m_terrainModel;
	Mouse*						m_mouse;
	Keyboard*					m_keyboard;
	bool						m_isActive;

public:
	CompositeController();

	virtual bool Initialize(IModelPtr pModel, Mouse* mouse, Keyboard* keyboard) override;
	virtual bool CanHandle(unsigned int message) const override;
	virtual void HandleMessage(unsigned int message, float* fparam, unsigned* uparam) override;
	virtual void SetTerrainModel(IModelPtr pModel) override;
	virtual void SetMouse(Mouse* mouse) override;
	virtual void SetKeyboard(Keyboard* keyboard) override;
	virtual void Shutdown() override;
	virtual bool IsActive() const  override;

	void AddController(IControllerPtr controller);
	void RemoveController(IControllerPtr controller);

private:
	void SetMessageSystem(MessageSystem* messageSystem);

};

typedef std::shared_ptr<CompositeController> CompositeControllerPtr;

#endif
