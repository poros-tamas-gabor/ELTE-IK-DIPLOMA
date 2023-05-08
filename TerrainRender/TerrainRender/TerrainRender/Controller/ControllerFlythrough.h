#ifndef CONTROLLER_FLYTHROUGH_H
#define CONTROLLER_FLYTHROUGH_H

///////////////////////////////////////////////////////////////////////////////
// ControllerFlythrough.h
// ======================
// A derived class of the IController interface that implements control logic for the Flythrough mode.
//
// In Flythrough mode, the user can play back a loaded camera trajectory and save the simulated images to disk. This mode can only be 
// activated if there is a loaded camera trajectory in the program. Only one of the Explore3d or Flythrough modes can be active at a time.
//
// This class is responsible for interpreting user input, updating the model, and coordinating with the view to display the updated 3D models
// during the Flythrough mode. 
//
// To use this class, it must be instantiated and registered with the ControllerRouter.
// Once registered, it will receive user input from the ControllerRouter and decide how to update the Model.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include "IController.h"
#include "MessageSystem.h"
#include "../Model/IModel.h"
#include "../View/IView.h"
#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"

class ControllerFlythrough : public IController
{
private:
	IModelPtr							m_terrainModel;
	IViewPtr							m_terrainView;
	MousePtr							m_mouse;
	KeyboardPtr							m_keyboard;
	ControllerMessageSystemPtr			m_messageSystem;
	
	FlythroughState						m_flythroughState;
	size_t								m_recordedPrevFrameNum	= 0;

	bool								m_isActive				= false;
	bool								m_isRunning				= false;
	bool								m_isRecording			= false;
	bool								m_IsOutputDirChoosed	= false;

public:
	ControllerFlythrough();
	ControllerFlythrough operator=(const ControllerFlythrough&) = delete;
	ControllerFlythrough(const ControllerFlythrough&) = delete;
	virtual ~ControllerFlythrough() {}

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
	void SetMessageSystem(ControllerMessageSystemPtr messageSystem);
};

typedef std::shared_ptr<ControllerFlythrough> ControllerFlythroughPtr;



#endif