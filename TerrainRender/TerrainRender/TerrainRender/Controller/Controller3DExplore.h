#ifndef CONTROLLER_3D_EXPLORE_H
#define CONTROLLER_3D_EXPLORE_H

///////////////////////////////////////////////////////////////////////////////
// Controller3DExplore.h
// =====================
// A derived class of the IController interface that implements control logic for the Explore3D mode.
//
// In Explore3D mode, the user can freely explore a loaded terrain model and examine its surface mesh from different angles.
// This enables them to examine the surface mesh from various angles and identify any potential errors or inaccuracies in the model.
// The Explore3D mode is the default mode when the program is launched. Only one of the Explore3d or Flythrough modes can be active at a time.
//
// This class is responsible for interpreting user input, updating the model, and coordinating with the view to display the updated 3D models during the Explore3D mode.
//
// To use this class, it must be instantiated and registered with the ControllerRouter.
// Once registered, it will receive user input from the ControllerRouter and decide how to update the Model.
//
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
//////////////////////////////////////////////////////////////////////////////

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
	IModelPtr							m_terrainModel;
	MousePtr							m_mouse;
	KeyboardPtr							m_keyboard;
	ControllerMessageSystemPtr			m_messageSystem;
	bool								m_isActive = true;

public:
	Controller3DExplore();
	Controller3DExplore operator=(const Controller3DExplore&) = delete;
	Controller3DExplore(const Controller3DExplore&) = delete;
	virtual ~Controller3DExplore() {}

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

private:
	void ControlMouse(const MouseEvent& e) const;
	void SetMessageSystem(ControllerMessageSystemPtr messageSystem);
};



#endif