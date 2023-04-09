#include "ControllerFlythrough.h"
#include "../Model/TerrainModel.h"
#include "../resource.h"


ControllerFlythrough::ControllerFlythrough()
{
	m_handledMsgs.push_back(IDC_TIME_ELAPSED);
	m_handledMsgs.push_back(IDCC_ACTIVATE_FLYTHROUGH);
	m_handledMsgs.push_back(IDCC_ACTIVATE_3DEXPLORE);
	m_handledMsgs.push_back(IDCC_SPEED_FLYTHROUGH);
	m_handledMsgs.push_back(IDCC_START_FLYTHROUGH);
	m_handledMsgs.push_back(IDCC_PAUSE_FLYTHROUGH);
	m_handledMsgs.push_back(IDCC_STOP_FLYTHROUGH);
	m_handledMsgs.push_back(IDCC_SET_FRAME_FLYTHROUGH);

}

void ControllerFlythrough::SetMessageSystem(MessageSystem* messageSystem)
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
		this->m_terrainModel->HandleFlythroughMode(IDM_CAMERA_TRAJECTORY_START, NULL, NULL);
		break;
	}

	case IDCC_START_FLYTHROUGH:
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
		this->m_terrainModel->HandleFlythroughMode(IDM_CAMERA_TRAJECTORY_STOP, NULL, NULL);
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
void ControllerFlythrough::SetTerrainModel(IModelPtr pModel)
{
	this->m_terrainModel = pModel;
}
void ControllerFlythrough::SetMouse(Mouse* mouse)
{
	this->m_mouse = mouse;
}
void ControllerFlythrough::SetKeyboard(Keyboard* keyboard)
{
	this->m_keyboard = keyboard;
}


bool ControllerFlythrough::IsActive() const
{
	return this->m_isActive;
}




bool ControllerFlythrough::Initialize(IModelPtr pModel, Mouse* mouse, Keyboard* keyboard)
{
	if (pModel == nullptr || mouse == nullptr || keyboard == nullptr)
	{
		return false;
	}
	this->SetTerrainModel(pModel);
	this->SetMouse(mouse);
	this->SetKeyboard(keyboard);
	return true;
}
void ControllerFlythrough::Shutdown() {}


