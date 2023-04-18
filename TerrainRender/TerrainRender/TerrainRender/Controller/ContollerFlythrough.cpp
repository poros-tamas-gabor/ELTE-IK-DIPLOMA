#include "ControllerFlythrough.h"
#include "../Model/TerrainModel.h"
#include "../resource.h"


ControllerFlythrough::ControllerFlythrough()
{
	m_handledMsgs.push_back(IDC_TIME_ELAPSED);
	m_handledMsgs.push_back(IDCC_ACTIVATE_FLYTHROUGH);
	m_handledMsgs.push_back(IDCC_ACTIVATE_3DEXPLORE);
	m_handledMsgs.push_back(IDCC_SPEED_FLYTHROUGH);
	m_handledMsgs.push_back(IDCC_PLAY_FLYTHROUGH);
	m_handledMsgs.push_back(IDCC_PAUSE_FLYTHROUGH);
	m_handledMsgs.push_back(IDCC_STOP_FLYTHROUGH);
	m_handledMsgs.push_back(IDCC_SET_FRAME_FLYTHROUGH);
	m_handledMsgs.push_back(IDCC_RECORD_FLYTHROUGH);
	m_handledMsgs.push_back(IDCC_STOP_RECORD_FLYTHROUGH);

}

void ControllerFlythrough::SetMessageSystem(ControllerMessageSystemPtr messageSystem)
{
	m_messageSystem = messageSystem;
}

bool ControllerFlythrough::CanHandle(unsigned int message) const
{
	auto it = std::find(m_handledMsgs.begin(), m_handledMsgs.end(), message);
	return it != m_handledMsgs.end();
}

void ControllerFlythrough::HandleMessage(unsigned int message, float* fparam, unsigned* uparam)
{
	switch (message)
	{

	case IDCC_ACTIVATE_3DEXPLORE:
	{
		this->m_isActive = false;
		this->m_isRunning = false;
		break;
	}
	case IDCC_ACTIVATE_FLYTHROUGH:
	{
		this->m_isActive = true;
		this->m_isRunning = false;
		this->m_terrainModel->HandleFlythroughMode(IDM_TRAJECTORY_START_POS, NULL, NULL);
		break;
	}

	case IDCC_PLAY_FLYTHROUGH:
	{
		this->m_isRunning = true;
		break;
	}

	case IDCC_PAUSE_FLYTHROUGH:
	{
		this->m_isRunning = false;
		break;
	}
	case IDCC_STOP_FLYTHROUGH:
	{
		this->m_isRunning = false;
		this->m_terrainModel->HandleFlythroughMode(IDM_TRAJECTORY_START_POS, NULL, NULL);
		break;
	}

	case IDCC_RECORD_FLYTHROUGH:
	{
		this->m_isRunning = true;
		this->m_isRecording = true;
		m_recordedPrevFrameNum = 0;
		break;
	}

	case IDCC_STOP_RECORD_FLYTHROUGH:
	{
		this->m_isRunning = false;
		this->m_isRecording = false;
		break;
	}


	case IDCC_SPEED_FLYTHROUGH:
	{
		if (fparam != nullptr)
		{
			this->m_speed = *fparam;
		}
		break;
	}
	case IDCC_SET_FRAME_FLYTHROUGH:
	{
		if (uparam != nullptr)
		{
			this->m_terrainModel->HandleFlythroughMode(IDM_CAMERA_TRAJECTORY_SET_FRAME, NULL, uparam);
		}
		break;
	}
	case IDC_TIME_ELAPSED:
	{
		if (fparam != nullptr)
		{
			float TimeEllapsed = 0;
			TimeEllapsed = *fparam;

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
				TimeEllapsed *= m_speed;
				this->m_terrainModel->HandleFlythroughMode(IDM_CAMERA_TRAJECTORY_NEXT_FRAME, &TimeEllapsed, NULL);


			}

			if (m_keyboard->KeyIsPressed(VK_SPACE))
			{
				m_isRunning ^= 1;
			}

			if (m_keyboard->KeyIsPressed(VK_ESCAPE))
			{
				this->m_terrainModel->HandleFlythroughMode(IDM_CAMERA_TRAJECTORY_STOP, NULL, NULL);
				m_isRunning = false;
			}
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

void ControllerFlythrough::HandleIModelState(const std::vector<IRenderableState>&) {}
void ControllerFlythrough::HandleIModelState(const FlythroughState& state) {
	m_flythroughState = state;
}
void ControllerFlythrough::HandleIModelState(const Explore3DState&) {}
void ControllerFlythrough::HandleIModelState(const CameraState&) {}


