#include "Controller3DExplore.h"
#include "../Model/TerrainModel.h"
#include "../resource.h"


Controller3DExplore::Controller3DExplore()
{
	m_handledMsgs.push_back(IDC_TIME_ELLAPSED);
}
bool Controller3DExplore::CanHandle(unsigned int message) const
{
	auto it = std::find(m_handledMsgs.begin(), m_handledMsgs.end(), message);
	return it != m_handledMsgs.end();
}

void Controller3DExplore::Control(unsigned int message, float* fparam, unsigned* uparam)
{
	float TimeEllapsed = 0;
	switch (message)
	{
	case IDC_TIME_ELLAPSED:
	{
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
				this->m_terrainModel->MoveCamera(IDC_CAMERA_MOVE_UP, TimeEllapsed);
			}
			if (m_keyboard->KeyIsPressed('C'))
			{
				this->m_terrainModel->MoveCamera(IDC_CAMERA_MOVE_DOWN, TimeEllapsed);
			}
			if (m_keyboard->KeyIsPressed('W'))
			{
				this->m_terrainModel->MoveCamera(IDC_CAMERA_MOVE_FORWARD, TimeEllapsed);
			}
			if (m_keyboard->KeyIsPressed('S'))
			{
				this->m_terrainModel->MoveCamera(IDC_CAMERA_MOVE_BACK, TimeEllapsed);
			}
			if (m_keyboard->KeyIsPressed('A'))
			{
				this->m_terrainModel->MoveCamera(IDC_CAMERA_MOVE_LEFT, TimeEllapsed);
			}
			if (m_keyboard->KeyIsPressed('D'))
			{
				this->m_terrainModel->MoveCamera(IDC_CAMERA_MOVE_RIGHT, TimeEllapsed);
			}
		}
	}
	default:
		break;
	}

}
void Controller3DExplore::SetTerrainModel(TerrainModel* pModel)
{
	this->m_terrainModel = pModel;
}
void Controller3DExplore::SetMouse(Mouse* mouse)
{
	this->m_mouse = mouse;
}
void Controller3DExplore::SetKeyboard(Keyboard* keyboard)
{
	this->m_keyboard = keyboard;
}

bool Controller3DExplore::Initialize(TerrainModel* pModel, Mouse* mouse, Keyboard* keyboard)
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
				this->m_terrainModel->RotateCamera(IDC_CAMERA_ROTATE, (float)pitch, (float)yaw);
			}

		}
		prev = e;
	}
	else if (MouseEvent::Type::RAW_MOVE_RELATIVE == e.GetType())
	{
		if (m_mouse->IsLeftDown())
		{
			this->m_terrainModel->RotateCamera(IDC_CAMERA_ROTATE, (float)e.GetPosY(), (float)e.GetPosX());
		}
	}
}
