#ifndef CONTROLLER_3D_EXPLORE_H
#define CONTROLLER_3D_EXPLORE_H

#include "IController.h"
#include "MessageSystem.h"
#include "../Model/IModel.h"
#include "../View/IView.h"
#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"

class TerrainModel;
class Controller3DExplore : public IController
{
private:
	IModelPtr				m_terrainModel;
	MousePtr				m_mouse;
	KeyboardPtr				m_keyboard;
	std::vector<unsigned>	m_handledMsgs;
	ControllerMessageSystemPtr			m_messageSystem;

	bool					m_isActive = true;


public:
	Controller3DExplore();
	virtual ~Controller3DExplore() {}

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

	void ControlMouse(const MouseEvent& e) const;
	void SetMessageSystem(ControllerMessageSystemPtr messageSystem);

};



#endif