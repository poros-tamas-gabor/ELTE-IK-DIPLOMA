#include "Controller3DExplore.h"
#include "../Model/TerrainModel.h"
#include "ControllerEvents.h"

Controller3DExplore::Controller3DExplore()
{
	m_handledEvents.push_back(ControllerEvent::NewFrame);
}
bool Controller3DExplore::CanHandle(ControllerEvent::IEvent* event) const
{
	auto it = std::find(m_handledEvents.begin(), m_handledEvents.end(), event->GetEventType());
	return it != m_handledEvents.end();
}

void Controller3DExplore::Control(ControllerEvent::IEvent* event)
{
	float TimeEllapsed = 0;
	if (event->GetEventType() == ControllerEvent::Type::NewFrame)
	{
		if (ControllerEvent::NewFrameEvent* castedEvent = dynamic_cast<ControllerEvent::NewFrameEvent*>(event))
		{
			TimeEllapsed = castedEvent->GetElapsedMiliseconds();
		}
		else
		{
			return;
		}
	}

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
		CameraMoveEvent::Type type = CameraMoveEvent::Type::MoveUp;
		this->_terrainModel->OnMoveCamera(CameraMoveEvent::Event(type, TimeEllapsed));
	}
	else if (m_keyboard->KeyIsPressed('C'))
	{
		CameraMoveEvent::Type type = CameraMoveEvent::Type::MoveDown;
		this->_terrainModel->OnMoveCamera(CameraMoveEvent::Event(type, TimeEllapsed));
	}
	else if (m_keyboard->KeyIsPressed('W'))
	{
		CameraMoveEvent::Type type = CameraMoveEvent::Type::MoveForward;
		this->_terrainModel->OnMoveCamera(CameraMoveEvent::Event(type, TimeEllapsed));

	}
	else if (m_keyboard->KeyIsPressed('S'))
	{
		CameraMoveEvent::Type type = CameraMoveEvent::Type::MoveBack;
		this->_terrainModel->OnMoveCamera(CameraMoveEvent::Event(type, TimeEllapsed));

	}
	else if (m_keyboard->KeyIsPressed('A'))
	{
		CameraMoveEvent::Type type = CameraMoveEvent::Type::MoveLeft;
		this->_terrainModel->OnMoveCamera(CameraMoveEvent::Event(type, TimeEllapsed));

	}
	else if (m_keyboard->KeyIsPressed('D'))
	{
		CameraMoveEvent::Type type = CameraMoveEvent::Type::MoveRight;
		this->_terrainModel->OnMoveCamera(CameraMoveEvent::Event(type, TimeEllapsed));
	}

}
void Controller3DExplore::SetTerrainModel(TerrainModel* pModel)
{
	this->_terrainModel = pModel;
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
				CameraRotateEvent::Event event(CameraRotateEvent::Type::Rotate, { (float)pitch, (float)yaw });

				this->_terrainModel->OnRotateCamera(event);
			}

		}
		prev = e;
	}
	else if (MouseEvent::Type::RAW_MOVE_RELATIVE == e.GetType())
	{
		if (m_mouse->IsLeftDown())
		{
			CameraRotateEvent::Event event(CameraRotateEvent::Type::Rotate, { (float)e.GetPosY(), (float)e.GetPosX() });
			this->_terrainModel->OnRotateCamera(event);
		}
	}
}
