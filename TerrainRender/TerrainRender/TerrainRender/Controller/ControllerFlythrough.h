#ifndef CONTROLLER_FLYTHROUGH_H
#define CONTROLLER_FLYTHROUGH_H

#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"
#include "IController.h"

class TerrainModel;
class ControllerFlythrough : public IController
{
private:
	TerrainModel* m_terrainModel;
	Mouse* m_mouse;
	Keyboard* m_keyboard;
	std::vector<unsigned>	m_handledMsgs;
	MessageSystem* m_messageSystem;

	bool					m_isActive = false;
	bool					m_isRunning = false;
	float					m_speed = 1.0f;


public:
	ControllerFlythrough();
	virtual ~ControllerFlythrough() {}

	virtual bool CanHandle(unsigned int message) const override;
	virtual void HandleMessage(unsigned int message, float* fparam, unsigned* uparam) override;
	virtual void SetTerrainModel(TerrainModel* pModel) override;
	virtual void SetMouse(Mouse* mouse) override;
	virtual void SetKeyboard(Keyboard* keyboard) override;

	virtual bool Initialize(TerrainModel* pModel, Mouse* mouse, Keyboard* keyboard) override;
	virtual bool IsActive() const  override;
	virtual void Shutdown() override;

private:

	void SetMessageSystem(MessageSystem* messageSystem);

};



#endif