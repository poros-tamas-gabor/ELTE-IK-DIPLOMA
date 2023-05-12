#include "ControllerFlythrough.h"
#include "../Model/TerrainModel.h"
#include "../ErrorHandler.h"
#include "../resource.h"


ControllerFlythrough::ControllerFlythrough() {}

bool ControllerFlythrough::IsActive() const
{
	return this->m_isActive;
}

bool ControllerFlythrough::Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard)
{
	if (pModel == nullptr || mouse == nullptr || keyboard == nullptr || pView == nullptr)
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

bool ControllerFlythrough::HandleMessage(IControllerMessageIDs message, const std::vector<float>& fparams, const std::vector<unsigned>& uparams)
{
	try {
		switch (message)
		{
		case IDCC_OUTPUT_DIR_CHOOSED:
		{
			m_IsOutputDirChoosed = true;
			return true;
		}
		case IDCC_IS_FLYTHROUGH_MODE_ON:
		{
			return IsActive();
		}
		case IDC_ACTIVATE_EXPLORE3D_MODE:
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
				IModelMessageIDs modelMessage = IDC2IDM(message);
				return this->m_terrainModel->HandleMessage(modelMessage, {}, fparams, uparams);
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
			THROW_TREXCEPTION_IF_FAILED(m_IsOutputDirChoosed, L"Screen capture failed because the output directory was not specified. To fix this, select the \"File > Set output directory\" menu item and choose the directory where you want to save the captured screen images.");
			this->m_isRunning = true;
			this->m_isRecording = true;
			m_recordedPrevFrameNum = 0;
			m_terrainView->HandleMessage(IDC2IDV(IDC_FLYTHROUGH_RECORD_START), {}, {}, {});
			return true;
		}
		case IDC_FLYTHROUGH_RECORD_STOP:
		{
			THROW_TREXCEPTION_IF_FAILED(m_IsOutputDirChoosed, L"Screen capture failed because the output directory was not specified. To fix this, select the \"File > Set output directory\" menu item and choose the directory where you want to save the captured screen images.");
			this->m_isRunning = false;
			this->m_isRecording = false;
			m_terrainView->HandleMessage(IDC2IDV(IDC_FLYTHROUGH_RECORD_STOP), {}, {}, {});
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
			if (!IsActive())
				return true;

			while (!m_keyboard->KeyBufferIsEmpty())
			{
				KeyboardEvent e = m_keyboard->ReadKey();
				if (e.GetKeyCode() == VK_SPACE && e.IsRelease())
				{
					m_isRunning ^= 1;
				}
				if (e.GetKeyCode() == 'R' && e.IsRelease())
				{
					if (m_isRecording)
						m_messageSystem->Publish(IDC_FLYTHROUGH_RECORD_STOP, fparams, uparams);
					else
						m_messageSystem->Publish(IDC_FLYTHROUGH_RECORD_START, fparams, uparams);
				}
				if (e.GetKeyCode() == VK_ESCAPE && e.IsRelease())
				{
					m_isRunning = false;
					return m_terrainModel->HandleMessage(IDM_FLYTHROUGH_STOP, {}, fparams, uparams);
				}

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
					this->m_terrainView->HandleMessage(IDV_CAPTURE_SCREEN, {}, {}, { m_flythroughState.currentFrame + 1});
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
	catch (const COMException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const TRException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const std::exception& e)
	{
		ErrorHandler::Log(e);
	}
	catch (...)
	{
		ErrorHandler::Log("Unknown Exception: No details available");
	}
	return false;

}

void ControllerFlythrough::SetMessageSystem(ControllerMessageSystemPtr messageSystem)
{
	m_messageSystem = messageSystem;
}
void ControllerFlythrough::SetTerrainView(IViewPtr pView) 
{
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

void ControllerFlythrough::HandleIModelState(const MeshGroupState&) {}
void ControllerFlythrough::HandleIModelState(const FlythroughState& state) { m_flythroughState = state; }
void ControllerFlythrough::HandleIModelState(const Explore3DState&) {}
void ControllerFlythrough::HandleIModelState(const GeneralModelState&) {}


