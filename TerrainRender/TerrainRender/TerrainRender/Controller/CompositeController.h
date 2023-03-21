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
	TerrainModel*				m_terrainModel;
	Mouse*						m_mouse;
	Keyboard*					m_keyboard;
	bool						m_isActive;

public:
	CompositeController();

	virtual bool Initialize(TerrainModel* pModel, Mouse* mouse, Keyboard* keyboard) override;
	virtual bool CanHandle(unsigned int message) const override;
	virtual void Control(unsigned int message, float* fparam, unsigned* uparam) override;
	virtual void SetTerrainModel(TerrainModel* pModel) override;
	virtual void SetMouse(Mouse* mouse) override;
	virtual void SetKeyboard(Keyboard* keyboard) override;
	virtual void Shutdown() override;
	virtual void Disable() override;
	virtual void Activate() override;
	virtual bool IsActive() const  override;

	void AddController(IControllerPtr controller);
	void RemoveController(IControllerPtr controller);

private:
	void SetMessageSystem(MessageSystem* messageSystem);

};

#endif
