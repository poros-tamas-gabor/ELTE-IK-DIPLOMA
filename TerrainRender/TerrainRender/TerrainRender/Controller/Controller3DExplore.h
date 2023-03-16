#ifndef CONTROLLER_3D_EXPLORE_H
#define CONTROLLER_3D_EXPLORE_H

#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"
#include "IController.h"

class TerrainModel;
class Controller3DExplore : public IController
{
private:
	TerrainModel* _terrainModel;
	Mouse* m_mouse;
	Keyboard* m_keyboard;
	std::vector<ControllerEvent::Type>	m_handledEvents;


public:
	Controller3DExplore();
	virtual ~Controller3DExplore() {}

	virtual bool CanHandle(ControllerEvent::IEvent* event) const override;
	virtual void Control(ControllerEvent::IEvent* event) override;
	virtual void SetTerrainModel(TerrainModel* pModel) override;
	virtual void SetMouse(Mouse* mouse) override;
	virtual void SetKeyboard(Keyboard* keyboard) override;

	virtual bool Initialize(TerrainModel* pModel, Mouse* mouse, Keyboard* keyboard) override;
	virtual void Shutdown() override;

private:

	void ControlMouse(const MouseEvent& e) const;
	//void ControlKeyboard(const KeyboardEvent& e, float dt) ;

};



#endif