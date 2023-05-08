#ifndef CONTROLLER_GUI_H
#define CONTROLLER_GUI_H

#include "IController.h"
#include "MessageSystem.h"
#include "../Model/IModel.h"
#include "../View/IView.h"
#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"
#include "../Model/Persistence/ICallable.h"

///////////////////////////////////////////////////////////////////////////////
// ControllerGui.h
// ===============
// A derived class of the IController interface that handles user input from the GUI and processes the corresponding commands.
// If the command received by the ControllerGui needs to be handled by another controller, it forwards the command to the appropriate controller via the ControllerMessageSystem.
//
// This class is always active and responsible for interpreting user input and translating it into appropriate commands for the model.
//
// To use this class, it must be instantiated and registered with the ControllerRouter.
// Once registered, it will receive user input from the GUI and delegate commands to other controllers if necessary, or update the model directly.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

class ControllerGui : public IController
{
private:
	IModelPtr								m_terrainModel;
	IViewPtr								m_terrainView;
	ControllerMessageSystemPtr				m_messageSystem;
	bool									m_isActive = true;

	const wchar_t* m_filter_stl		=	L"Binary stl files\0*.stl\0All\0*.*\0";
	const wchar_t* m_filter_csv		=	L"Trajectory csv files\0*.csv\0All\0*.*\0";
	const wchar_t* m_filter_json	=	L"Configuration json files\0*.json\0All\0*.*\0";

public:
	ControllerGui();
	ControllerGui operator=(const ControllerGui&) = delete;
	ControllerGui(const ControllerGui&) = delete;
	virtual ~ControllerGui();

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
	void LoadFiles(IModelMessageIDs message, const std::vector<std::wstring>& sparam, const std::vector<float>& fparam, const std::vector<unsigned>& uparam);
	void SetMessageSystem(ControllerMessageSystemPtr messageSystem);
};



#endif