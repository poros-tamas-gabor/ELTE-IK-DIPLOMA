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
	IModelPtr					m_terrainModel;
	IViewPtr					m_terrainView;
	std::vector<unsigned>		m_handledMsgs;
	ControllerMessageSystemPtr	m_messageSystem;
	bool						m_isActive;

	const wchar_t* m_filter_stl =	L"Binary stl files\0*.stl\0All\0*.*\0";
	const wchar_t* m_filter_csv =	L"Trajectory csv files\0*.csv\0All\0*.*\0";
	const wchar_t* m_filter_json =	L"Configuration json files\0*.json\0All\0*.*\0";

public:
	GuiController();
	virtual ~GuiController();

	virtual bool CanHandle(unsigned int message) const override;
	virtual void HandleMessage(unsigned int message, float* fparam, unsigned* uparam) override;
	virtual void SetTerrainModel(IModelPtr pModel) override;
	virtual void SetMouse(MousePtr mouse) override;
	virtual void SetKeyboard(KeyboardPtr keyboard) override;
	virtual void SetTerrainView(IViewPtr pView) override;

	virtual bool Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard) override;
	virtual bool IsActive() const  override;
	virtual bool IsFlythroughModeOn(void) const override;
	virtual void Shutdown() override;

	void HandleIModelState(const std::vector<IRenderableState>&) override;
	void HandleIModelState(const FlythroughState&) override;
	void HandleIModelState(const Explore3DState&) override;
	void HandleIModelState(const CameraState&) override;

private:
	void OpenFileDialog(wchar_t* filePath, unsigned buffer, const wchar_t* filter);
	void OpenFileDialogDirectory(std::wstring& directory);
	void OpenFileDialogMultipleSelection(std::vector<std::wstring>& files, const wchar_t* filter);
	void StartWorkerThread(const ICallableCreator& creator, std::atomic_bool& running);


	void SetMessageSystem(ControllerMessageSystemPtr messageSystem);
};



#endif