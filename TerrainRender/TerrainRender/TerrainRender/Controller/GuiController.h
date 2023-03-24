#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include "IController.h"
class GuiController : public IController
{
private:
	TerrainModel*				m_terrainModel;
	std::vector<unsigned>		m_handledMsgs;
	MessageSystem*				m_messageSystem;
	bool						m_isActive;

public:
	GuiController();
	virtual ~GuiController();

	virtual bool CanHandle(unsigned int message) const override;
	virtual void HandleMessage(unsigned int message, float* fparam, unsigned* uparam) override;
	virtual void SetTerrainModel(TerrainModel* pModel) override;
	virtual void SetMouse(Mouse* mouse) override;
	virtual void SetKeyboard(Keyboard* keyboard) override;

	virtual bool Initialize(TerrainModel* pModel, Mouse* mouse, Keyboard* keyboard) override;
	virtual bool IsActive() const  override;
	virtual void Shutdown() override;

private:
	void OpenFileDialog(wchar_t* filePath, unsigned buffer);
	void OpenFileDialogMultipleSelection(std::vector<std::wstring>& files);

	void SetMessageSystem(MessageSystem* messageSystem);
};



#endif