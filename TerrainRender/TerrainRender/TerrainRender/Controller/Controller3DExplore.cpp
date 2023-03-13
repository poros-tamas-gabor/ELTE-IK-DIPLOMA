#include "Controllers.h"
#include "../Graphics/Graphics.h"
#include "ControllerContainer.h" 

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

	if (_container->_keyboard->KeyIsPressed(VK_SPACE))
	{
		this->_graphics->_position.MoveUp(dt);
	}
	if (_container->_keyboard->KeyIsPressed('C'))
	{
		this->_graphics->_position.MoveDown(dt);
	}
	if (_container->_keyboard->KeyIsPressed('W'))
	{
		this->_graphics->_position.MoveForward(dt);
	}
	if (_container->_keyboard->KeyIsPressed('S'))
	{
		this->_graphics->_position.MoveBack(dt);
	}
	if (_container->_keyboard->KeyIsPressed('A'))
	{
		this->_graphics->_position.MoveLeft(dt);
	}
	if (_container->_keyboard->KeyIsPressed('D'))
	{
		this->_graphics->_position.MoveRight(dt);
	}
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
				this->_graphics->_position.RotatePitchYaw((float)pitch, (float)yaw);
			}

		}
		prev = e;
	}
	else if (MouseEvent::Type::RAW_MOVE_RELATIVE == e.GetType())
	{
		if (_container->_mouse->IsLeftDown())
		{
			this->_graphics->_position.RotatePitchYaw((float)e.GetPosY(), (float)e.GetPosX());
		}
	}
}
