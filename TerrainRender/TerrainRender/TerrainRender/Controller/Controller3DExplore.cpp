#include "Controller3DExplore.h"
#include "../Model/TerrainModel.h"
#include "../resource.h"


Controller3DExplore::Controller3DExplore()
{
	m_handledMsgs.push_back(IDC_TIME_ELAPSED);
	m_handledMsgs.push_back(IDCC_ACTIVATE_FLYTHROUGH);
	m_handledMsgs.push_back(IDCC_ACTIVATE_3DEXPLORE);

}

void Controller3DExplore::SetMessageSystem(ControllerMessageSystemPtr messageSystem)
{
	m_messageSystem = messageSystem;
}

bool Controller3DExplore::CanHandle(unsigned int message) const
{
	auto it = std::find(m_handledMsgs.begin(), m_handledMsgs.end(), message);
	return it != m_handledMsgs.end();
}

void Controller3DExplore::HandleMessage(unsigned int message, float* fparam, unsigned* uparam)
{
	float TimeEllapsed = 0;
	switch (message)
	{
	case IDCC_ACTIVATE_3DEXPLORE:
	{
		this->m_isActive = true;
		break;
	}
	case IDCC_ACTIVATE_FLYTHROUGH:
	{
		this->m_isActive = false;
		break;
	}
	case IDC_TIME_ELAPSED:
	{
		if (!IsActive())
			return;
		if(fparam != nullptr)
		{
			TimeEllapsed = *fparam;

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
				this->m_terrainModel->HandleExplore3DMode(IDM_CAMERA_MOVE_UP, &TimeEllapsed);
			}
			if (m_keyboard->KeyIsPressed('C'))
			{
				this->m_terrainModel->HandleExplore3DMode(IDM_CAMERA_MOVE_DOWN, &TimeEllapsed);
			}
			if (m_keyboard->KeyIsPressed('W'))
			{
				this->m_terrainModel->HandleExplore3DMode(IDM_CAMERA_MOVE_FORWARD, &TimeEllapsed);
			}
			if (m_keyboard->KeyIsPressed('S'))
			{
				this->m_terrainModel->HandleExplore3DMode(IDM_CAMERA_MOVE_BACK, &TimeEllapsed);
			}
			if (m_keyboard->KeyIsPressed('A'))
			{
				this->m_terrainModel->HandleExplore3DMode(IDM_CAMERA_MOVE_LEFT, &TimeEllapsed);
			}
			if (m_keyboard->KeyIsPressed('D'))
			{
				this->m_terrainModel->HandleExplore3DMode(IDM_CAMERA_MOVE_RIGHT, &TimeEllapsed);
			}

		}
		break;
		
	}
	default:
		break;
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
	if (pModel.get() == nullptr || mouse.get() == nullptr || keyboard.get() == nullptr)
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
				float rotates[2] = { (float)pitch, (float)yaw };
				this->m_terrainModel->HandleExplore3DMode(IDM_CAMERA_ROTATE, rotates);
			}

		}
		prev = e;
	}
	else if (MouseEvent::Type::RAW_MOVE_RELATIVE == e.GetType())
	{
		if (m_mouse->IsLeftDown())
		{
			float rotates[2] = { (float)e.GetPosY(), (float)e.GetPosX() };
			this->m_terrainModel->HandleExplore3DMode(IDM_CAMERA_ROTATE, rotates);
		}
	}
}
