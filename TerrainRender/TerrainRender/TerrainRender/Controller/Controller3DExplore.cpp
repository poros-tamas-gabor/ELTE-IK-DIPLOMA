#include "Controller3DExplore.h"
#include "../Model/TerrainModel.h"
#include "../resource.h"


Controller3DExplore::Controller3DExplore() {}

void Controller3DExplore::SetMessageSystem(ControllerMessageSystemPtr messageSystem)
{
	m_messageSystem = messageSystem;
}

bool Controller3DExplore::HandleMessage(IControllerMessageIDs message, const std::vector<float>& fparams, const std::vector<unsigned>& uparams)
{
	switch (message)
	{
	case IDCC_IS_FLYTHROUGH_MODE_ON:
	{
		return !IsActive();
	}
	case IDC_ACTIVATE_3DEXPLORE_MODE:
	{
		this->m_isActive = true;
		return true;
	}
	case IDC_ACTIVATE_FLYTHROUGH_MODE:
	{
		if (m_terrainModel->IsTrajectoryInitialized())
		{
			this->m_isActive = false;
			return true;
		}
		return false;
	}
	case IDC_E3D_CAMERA_SPEED:
	case IDC_E3D_ROTATION_SPEED:
	case IDC_E3D_CAMERA_RESET:
	{
		IModelMessageIDs modelMessage = IDC2IDM(message);
		return this->m_terrainModel->HandleMessage(modelMessage, {}, fparams, uparams);
	}
	case IDC_TIME_ELAPSED:
	{
		if (!IsActive())
			return true;

		while (!m_keyboard->KeyBufferIsEmpty())
		{
			KeyboardEvent e = m_keyboard->ReadKey();
		}
		while (!m_keyboard->CharBufferIsEmpty())
		{
			unsigned char c = m_keyboard->ReadChar();
		}

		while (!m_mouse->EventBufferIsEmpty())
		{
			MouseEvent e = m_mouse->ReadEvent();
			ControlMouse(e);
		}

		if (m_keyboard->KeyIsPressed(VK_SPACE))
		{
			this->m_terrainModel->HandleMessage(IDM_E3D_MOVE_UP, {},fparams, uparams);
		}
		if (m_keyboard->KeyIsPressed('C'))
		{
			this->m_terrainModel->HandleMessage(IDM_E3D_MOVE_DOWN, {}, fparams, uparams);
		}
		if (m_keyboard->KeyIsPressed('W'))
		{
			this->m_terrainModel->HandleMessage(IDM_E3D_MOVE_FORWARD, {}, fparams, uparams);
		}
		if (m_keyboard->KeyIsPressed('S'))
		{
			this->m_terrainModel->HandleMessage(IDM_E3D_MOVE_BACK, {}, fparams, uparams);
		}
		if (m_keyboard->KeyIsPressed('A'))
		{
			this->m_terrainModel->HandleMessage(IDM_E3D_MOVE_LEFT, {}, fparams, uparams);
		}
		if (m_keyboard->KeyIsPressed('D'))
		{
			this->m_terrainModel->HandleMessage(IDM_E3D_MOVE_RIGHT, {}, fparams, uparams);
		}
		return true;
		
	}
	default:
		return true;
	}

}
void Controller3DExplore::SetTerrainView(IViewPtr pView) {}
void Controller3DExplore::SetTerrainModel(IModelPtr pModel)
{
	this->m_terrainModel = pModel;
}
void Controller3DExplore::SetMouse(MousePtr mouse)
{
	this->m_mouse = mouse;
}
void Controller3DExplore::SetKeyboard(KeyboardPtr keyboard)
{
	this->m_keyboard = keyboard;
}

bool Controller3DExplore::IsActive() const
{
	return this->m_isActive;
}


bool Controller3DExplore::Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard)
{
	if (pModel == nullptr || mouse == nullptr || keyboard == nullptr || pView == nullptr)
	{
		return false;
	}
	this->SetTerrainModel(pModel);
	this->SetMouse(mouse);
	this->SetKeyboard(keyboard);
	return true;
}
void Controller3DExplore::Shutdown() {}

void Controller3DExplore::ControlMouse(const MouseEvent& e) const
{
	if (MouseEvent::Type::RAW_MOVE_ABSOLUTE == e.GetType())
	{
		static MouseEvent prev;
		if (prev.IsValid())
		{
			int yaw = e.GetPosX() - prev.GetPosX();
			int pitch = e.GetPosY() - prev.GetPosY();

			if (m_mouse->IsLeftDown())
			{
				std::vector<float> rotates = { (float)pitch, (float)yaw };
				this->m_terrainModel->HandleMessage(IDM_E3D_ROTATE, {}, rotates, {});
			}

		}
		prev = e;
	}
	else if (MouseEvent::Type::RAW_MOVE_RELATIVE == e.GetType())
	{
		if (m_mouse->IsLeftDown())
		{
			std::vector<float> rotates = { (float)e.GetPosY(), (float)e.GetPosX() };
			this->m_terrainModel->HandleMessage(IDM_E3D_ROTATE, {}, rotates, {});
		}
	}
}

void Controller3DExplore::HandleIModelState(const MeshGroupState&) {}
void Controller3DExplore::HandleIModelState(const FlythroughState&) {}
void Controller3DExplore::HandleIModelState(const Explore3DState&) {}
void Controller3DExplore::HandleIModelState(const GeneralModelState&) {}
