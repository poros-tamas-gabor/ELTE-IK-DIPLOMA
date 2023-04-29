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
	IModelPtr							m_terrainModel;
	IViewPtr							m_terrainView;
	MousePtr							m_mouse;
	KeyboardPtr							m_keyboard;
	std::vector<IControllerMessageIDs>	m_handledMsgs;
	ControllerMessageSystemPtr			m_messageSystem;
	
	FlythroughState						m_flythroughState;
	size_t								m_recordedPrevFrameNum = 0;

	bool								m_isActive = false;
	bool								m_isRunning = false;
	bool								m_isRecording = false;
	float								m_speed = 1.0f;


public:
	ControllerFlythrough();
	virtual ~ControllerFlythrough() {}

	virtual bool CanHandle(IControllerMessageIDs message) const override;
	virtual void HandleMessage(IControllerMessageIDs message, const std::vector<float>& fparam, const std::vector<unsigned>& uparam) override;
	virtual void SetTerrainModel(IModelPtr pModel) override;
	virtual void SetMouse(MousePtr mouse) override;
	virtual void SetKeyboard(KeyboardPtr keyboard) override;
	virtual void SetTerrainView(IViewPtr pView) override;

	virtual bool Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard) override;
	virtual bool IsActive() const  override;
	virtual bool IsFlythroughModeOn(void) const override;
	virtual void Shutdown() override;

	void HandleIModelState(const MeshGroupState&) override;
	void HandleIModelState(const FlythroughState&) override;
	void HandleIModelState(const Explore3DState&) override;
	void HandleIModelState(const CameraState&) override;

private:

	void SetMessageSystem(ControllerMessageSystemPtr messageSystem);

};

typedef std::shared_ptr<ControllerFlythrough> ControllerFlythroughPtr;



#endif