#include "Controllers.h"
#include "../Graphics/Graphics.h"
#include "ControllerContainer.h"
#include "CameraMoveEvent.h"
#include "CameraRotateEvent.h"

void Controller3DExplore::Control(float dt) const
{
	while (!_container->_keyboard->KeyBufferIsEmpty())
	{
		KeyboardEvent e = _container->_keyboard->ReadKey();
	}
	while (!_container->_keyboard->CharBufferIsEmpty())
	{
		unsigned char c = _container->_keyboard->ReadChar();
	}

	while (!_container->_mouse->EventBufferIsEmpty())
	{
		MouseEvent e = _container->_mouse->ReadEvent();
		ControlMouse(e);	
	}

	CameraMoveEvent::Type type(CameraMoveEvent::Invalid);
	if (_container->_keyboard->KeyIsPressed(VK_SPACE))
	{
		type = CameraMoveEvent::Type::MoveUp;
		//this->_graphics->_position.MoveUp(dt);
	}
	else if (_container->_keyboard->KeyIsPressed('C'))
	{
		type = CameraMoveEvent::Type::MoveDown;
		//this->_graphics->_position.MoveDown(dt);
	}
	else if (_container->_keyboard->KeyIsPressed('W'))
	{
		type = CameraMoveEvent::Type::MoveForward;

		//this->_graphics->_position.MoveForward(dt);
	}
	else if (_container->_keyboard->KeyIsPressed('S'))
	{
		type = CameraMoveEvent::Type::MoveBack;

		//this->_graphics->_position.MoveBack(dt);
	}
	else if (_container->_keyboard->KeyIsPressed('A'))
	{
		type = CameraMoveEvent::Type::MoveLeft;

		//this->_graphics->_position.MoveLeft(dt);
	}
	else if (_container->_keyboard->KeyIsPressed('D'))
	{
		type = CameraMoveEvent::Type::MoveRight;
		//this->_graphics->_position.MoveRight(dt);
	}
	this->_graphics->OnMoveCamera(CameraMoveEvent::Event(type, dt));
}

Controller3DExplore::Controller3DExplore(ControllerContainer* container, ModelLayer* model, Graphics* graphics) : _container(container), _model(model), _graphics(graphics) {}


void Controller3DExplore::ControlMouse(const MouseEvent& e) const
{
	if (MouseEvent::Type::RAW_MOVE_ABSOLUTE == e.GetType())
	{
		static MouseEvent prev;
		if (prev.IsValid())
		{
			int yaw = e.GetPosX() - prev.GetPosX();
			int pitch = e.GetPosY() - prev.GetPosY();

			if (_container->_mouse->IsLeftDown())
			{
				CameraRotateEvent::Event event(CameraRotateEvent::Type::Rotate, { (float)pitch, (float)yaw });

				this->_graphics->OnRotateCamera(event);
				//this->_graphics->_position.RotatePitchYaw((float)pitch, (float)yaw);
			}

		}
		prev = e;
	}
	else if (MouseEvent::Type::RAW_MOVE_RELATIVE == e.GetType())
	{
		if (_container->_mouse->IsLeftDown())
		{
			CameraRotateEvent::Event event(CameraRotateEvent::Type::Rotate, { (float)e.GetPosY(), (float)e.GetPosX() });
			this->_graphics->OnRotateCamera(event);

			//this->_graphics->_position.RotatePitchYaw((float)e.GetPosY(), (float)e.GetPosX());
		}
	}
}
