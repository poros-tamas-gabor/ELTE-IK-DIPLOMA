#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include "IController.h"
#include "MessageSystem.h"
#include "../Model/IModel.h"
#include "../View/IView.h"
#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"
#include "../Model/Persistence/ICallable.h"
class GuiController : public IController
{
private:
	IModelPtr								m_terrainModel;
	IViewPtr								m_terrainView;
	ControllerMessageSystemPtr				m_messageSystem;
	bool									m_isActive		= true;

	const wchar_t* m_filter_stl =	L"Binary stl files\0*.stl\0All\0*.*\0";
	const wchar_t* m_filter_csv =	L"Trajectory csv files\0*.csv\0All\0*.*\0";
	const wchar_t* m_filter_json =	L"Configuration json files\0*.json\0All\0*.*\0";

public:
	GuiController();
	virtual ~GuiController();

	virtual bool HandleMessage(IControllerMessageIDs message, const std::vector<float>& fparam, const std::vector<unsigned>& uparam) override;
	virtual void SetTerrainModel(IModelPtr pModel) override;
	virtual void SetMouse(MousePtr mouse) override;
	virtual void SetKeyboard(KeyboardPtr keyboard) override;
	virtual void SetTerrainView(IViewPtr pView) override;

	virtual bool Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard) override;
	virtual bool IsActive() const  override;
	virtual void Shutdown() override;

	void HandleIModelState(const MeshGroupState&) override;
	void HandleIModelState(const FlythroughState&) override;
	void HandleIModelState(const Explore3DState&) override;
	void HandleIModelState(const GeneralModelState&) override;

private:
	void LoadFiles(IModelMessageIDs message, const std::vector<std::wstring>& sparam, const std::vector<float>& fparam, const std::vector<unsigned>& uparam);
	void SetMessageSystem(ControllerMessageSystemPtr messageSystem);
};



#endif