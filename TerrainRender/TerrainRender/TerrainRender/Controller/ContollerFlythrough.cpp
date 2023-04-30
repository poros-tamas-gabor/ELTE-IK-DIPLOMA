#include "ControllerFlythrough.h"
#include "../Model/TerrainModel.h"
#include "../resource.h"


ControllerFlythrough::ControllerFlythrough()
{

}

void ControllerFlythrough::SetMessageSystem(ControllerMessageSystemPtr messageSystem)
{
	m_messageSystem = messageSystem;
}



bool ControllerFlythrough::HandleMessage(IControllerMessageIDs message, const std::vector<float>& fparams, const std::vector<unsigned>& uparams)
{
	switch (message)
	{
	case IDCC_IS_FLYTHROUGH_MODE_ON:
	{
		return IsActive();
	}
	case IDC_ACTIVATE_3DEXPLORE_MODE:
	{
		this->m_isActive = false;
		this->m_isRunning = false;
		return true;
	}
	case IDC_ACTIVATE_FLYTHROUGH_MODE:
	{
		if (m_terrainModel->IsTrajectoryInitialized())
		{
			this->m_isActive = true;
			this->m_isRunning = false;
			this->m_terrainModel->HandleMessage(IDM_FLYTHROUGH_START_POSITION, {}, fparams, uparams);
			return true;
		}
		return false;
	}
	case IDC_FLYTHROUGH_START:
	{
		this->m_isRunning = true;
		return true;
	}

	case IDC_FLYTHROUGH_PAUSE:
	{
		this->m_isRunning = false;
		return true;
	}
	case IDC_FLYTHROUGH_STOP:
	{
		this->m_isRunning = false;
		return this->m_terrainModel->HandleMessage(IDM_FLYTHROUGH_START_POSITION, {}, fparams, uparams);
	}
	case IDC_FLYTHROUGH_RECORD_START:
	{
		this->m_isRunning = true;
		this->m_isRecording = true;
		m_recordedPrevFrameNum = 0;
		return true;
	}
	case IDC_FLYTHROUGH_RECORD_STOP:
	{
		this->m_isRunning = false;
		this->m_isRecording = false;
		return true;
	}
	case IDC_FLYTHROUGH_SET_SPEED:
	case IDC_FLYTHROUGH_SET_FRAME:
	{
		IModelMessageIDs modelMsg = IDC2IDM(message);
		return this->m_terrainModel->HandleMessage(modelMsg, {}, fparams, uparams);
	}
	case IDC_TIME_ELAPSED:
	{
		while (!m_keyboard->KeyBufferIsEmpty())
		{
			KeyboardEvent e = m_keyboard->ReadKey();
		}
		while (!m_keyboard->CharBufferIsEmpty())
		{
			unsigned char c = m_keyboard->ReadChar();
		}

		if (m_keyboard->KeyIsPressed(VK_SPACE))
		{
			m_isRunning ^= 1;
		}

		if (m_keyboard->KeyIsPressed(VK_ESCAPE))
		{
			m_isRunning = false;
			return m_terrainModel->HandleMessage(IDM_FLYTHROUGH_STOP, {}, fparams, uparams);
		}

		if (m_isActive && m_isRunning)
		{
			if (m_isRecording && (m_recordedPrevFrameNum == 0 || m_recordedPrevFrameNum != m_flythroughState.currentFrame))
			{
				m_recordedPrevFrameNum = m_flythroughState.currentFrame;
				this->m_terrainView->CaptureScreen(m_flythroughState.currentFrame);
			}
			m_isRunning = m_terrainModel->HandleMessage(IDM_FLYTHROUGH_NEXT_FRAME, {}, fparams, uparams);
			return m_isRunning;
		}
		return true;
	}
	default:
		return true;
	}

}

void ControllerFlythrough::SetTerrainView(IViewPtr pView) {
	this->m_terrainView = pView;
}

void ControllerFlythrough::SetTerrainModel(IModelPtr pModel)
{
	this->m_terrainModel = pModel;
}
void ControllerFlythrough::SetMouse(MousePtr mouse)
{
	this->m_mouse = mouse;
}
void ControllerFlythrough::SetKeyboard(KeyboardPtr keyboard)
{
	this->m_keyboard = keyboard;
}


bool ControllerFlythrough::IsActive() const
{
	return this->m_isActive;
}

bool ControllerFlythrough::Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard)
{
	if (pModel.get() == nullptr || mouse.get() == nullptr || keyboard.get() == nullptr )
	{
		return false;
	}
	this->SetTerrainModel(pModel);
	this->SetMouse(mouse);
	this->SetKeyboard(keyboard);
	this->SetTerrainView(pView);
	return true;
}
void ControllerFlythrough::Shutdown() {}

void ControllerFlythrough::HandleIModelState(const MeshGroupState&) {}
void ControllerFlythrough::HandleIModelState(const FlythroughState& state) {
	m_flythroughState = state;
}
void ControllerFlythrough::HandleIModelState(const Explore3DState&) {}
void ControllerFlythrough::HandleIModelState(const GeneralModelState&) {}


