#include "ControllerFlythrough.h"
#include "../Model/TerrainModel.h"
#include "../resource.h"


ControllerFlythrough::ControllerFlythrough()
{
	m_handledMsgs.push_back(IDC_TIME_ELAPSED);
	m_handledMsgs.push_back(IDC_ACTIVATE_3DEXPLORE_MODE);
	m_handledMsgs.push_back(IDC_ACTIVATE_FLYTHROUGH_MODE);
	m_handledMsgs.push_back(IDC_FLYTHROUGH_START);
	m_handledMsgs.push_back(IDC_FLYTHROUGH_PAUSE);
	m_handledMsgs.push_back(IDC_FLYTHROUGH_STOP);
	m_handledMsgs.push_back(IDC_FLYTHROUGH_RECORD_START);
	m_handledMsgs.push_back(IDC_FLYTHROUGH_RECORD_STOP);
	m_handledMsgs.push_back(IDC_FLYTHROUGH_SET_SPEED);
	m_handledMsgs.push_back(IDC_FLYTHROUGH_SET_FRAME);

}

void ControllerFlythrough::SetMessageSystem(ControllerMessageSystemPtr messageSystem)
{
	m_messageSystem = messageSystem;
}

bool ControllerFlythrough::CanHandle(IControllerMessageIDs message) const
{
	auto it = std::find(m_handledMsgs.begin(), m_handledMsgs.end(), message);
	return it != m_handledMsgs.end();
}

void ControllerFlythrough::HandleMessage(IControllerMessageIDs message, const std::vector<float>& fparams, const std::vector<unsigned>& uparams)
{
	switch (message)
	{
	case IDC_ACTIVATE_3DEXPLORE_MODE:
	{
		this->m_isActive = false;
		this->m_isRunning = false;
		break;
	}
	case IDC_ACTIVATE_FLYTHROUGH_MODE:
	{
		if (m_terrainModel->IsTrajectoryInitialized())
		{
			this->m_isActive = true;
			this->m_isRunning = false;
			this->m_terrainModel->HandleMessage(IDM_FLYTHROUGH_START_POSITION, {}, fparams, uparams);
		}
		break;
	}
	case IDC_FLYTHROUGH_START:
	{
		this->m_isRunning = true;
		break;
	}

	case IDC_FLYTHROUGH_PAUSE:
	{
		this->m_isRunning = false;
		break;
	}
	case IDC_FLYTHROUGH_STOP:
	{
		this->m_isRunning = false;
		this->m_terrainModel->HandleMessage(IDM_FLYTHROUGH_START_POSITION, {}, fparams, uparams);
		break;
	}
	case IDC_FLYTHROUGH_RECORD_START:
	{
		this->m_isRunning = true;
		this->m_isRecording = true;
		m_recordedPrevFrameNum = 0;
		break;
	}
	case IDC_FLYTHROUGH_RECORD_STOP:
	{
		this->m_isRunning = false;
		this->m_isRecording = false;
		break;
	}
	case IDC_FLYTHROUGH_SET_SPEED:
	{
		this->m_terrainModel->HandleMessage(IDM_FLYTHROUGH_SET_SPEED, {}, fparams, uparams);
		break;
	}
	case IDC_FLYTHROUGH_SET_FRAME:
	{
		this->m_terrainModel->HandleMessage(IDM_FLYTHROUGH_SET_FRAME, {}, fparams, uparams); 	
		break;
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

		if (m_isActive && m_isRunning)
		{
			if (m_isRecording && (m_recordedPrevFrameNum == 0 || m_recordedPrevFrameNum != m_flythroughState.currentFrame))
			{
				m_recordedPrevFrameNum = m_flythroughState.currentFrame;
				this->m_terrainView->CaptureScreen(m_flythroughState.currentFrame);
			}
			this->m_terrainModel->HandleMessage(IDM_FLYTHROUGH_NEXT_FRAME, {}, fparams, uparams);
		}

		if (m_keyboard->KeyIsPressed(VK_SPACE))
		{
			m_isRunning ^= 1;
		}

		if (m_keyboard->KeyIsPressed(VK_ESCAPE))
		{
			this->m_terrainModel->HandleMessage(IDM_FLYTHROUGH_STOP, {}, fparams, uparams);
			m_isRunning = false;
		}
		
		break;
	}
	default:
		break;
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
bool ControllerFlythrough::IsFlythroughModeOn(void) const
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
void ControllerFlythrough::HandleIModelState(const CameraState&) {}


