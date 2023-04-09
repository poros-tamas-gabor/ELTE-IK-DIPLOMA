#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include "IController.h"
#include "MessageSystem.h"
#include "../Model/IModel.h"
#include "../View/IView.h"
#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"
class GuiController : public IController
{
private:
	IModelPtr					m_terrainModel;
	std::vector<unsigned>		m_handledMsgs;
	ControllerMessageSystemPtr				m_messageSystem;
	bool						m_isActive;

public:
	GuiController();
	virtual ~GuiController();

	virtual bool CanHandle(unsigned int message) const override;
	virtual void HandleMessage(unsigned int message, float* fparam, unsigned* uparam) override;
	virtual void SetTerrainModel(IModelPtr pModel) override;
	virtual void SetMouse(MousePtr mouse) override;
	virtual void SetKeyboard(KeyboardPtr keyboard) override;

	virtual bool Initialize(IModelPtr pModel, MousePtr mouse, KeyboardPtr keyboard) override;
	virtual bool IsActive() const  override;
	virtual void Shutdown() override;

private:
	void OpenFileDialog(wchar_t* filePath, unsigned buffer);
	void OpenFileDialogMultipleSelection(std::vector<std::wstring>& files);

	void SetMessageSystem(ControllerMessageSystemPtr messageSystem);
};



#endif