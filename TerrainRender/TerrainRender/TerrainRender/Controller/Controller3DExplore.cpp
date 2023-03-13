#include "Controllers.h"
#include "../Graphics/Graphics.h"

void Controller3DExplore::Control(float dt, Graphics* graphics) const
{
	while (!_keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent e = _keyboard.ReadKey();
	}
	while (!_keyboard.CharBufferIsEmpty())
	{
		unsigned char c = _keyboard.ReadChar();
	}

	while (!_mouse.EventBufferIsEmpty())
	{
		MouseEvent e = _mouse.ReadEvent();
		ControlMouse(e, graphics);	
	}

	if (_keyboard.KeyIsPressed(VK_SPACE))
	{
		graphics->_position.MoveUp(dt);
	}
	if (_keyboard.KeyIsPressed('C'))
	{
		graphics->_position.MoveDown(dt);
	}
	if (_keyboard.KeyIsPressed('W'))
	{
		graphics->_position.MoveForward(dt);
	}
	if (_keyboard.KeyIsPressed('S'))
	{
		graphics->_position.MoveBack(dt);
	}
	if (_keyboard.KeyIsPressed('A'))
	{
		graphics->_position.MoveLeft(dt);
	}
	if (_keyboard.KeyIsPressed('D'))
	{
		graphics->_position.MoveRight(dt);
	}
}

void Controller3DExplore::ControlMouse(const MouseEvent& e, Graphics* graphics) const
{
	if (MouseEvent::Type::RAW_MOVE_ABSOLUTE == e.GetType())
	{
		static MouseEvent prev;
		if (prev.IsValid())
		{
			int yaw = e.GetPosX() - prev.GetPosX();
			int pitch = e.GetPosY() - prev.GetPosY();

			if (_mouse.IsLeftDown())
			{
				graphics->_position.RotatePitchYaw((float)pitch, (float)yaw);
			}

		}
		prev = e;
	}
	else if (MouseEvent::Type::RAW_MOVE_RELATIVE == e.GetType())
	{
		if (_mouse.IsLeftDown())
		{
			graphics->_position.RotatePitchYaw((float)e.GetPosY(), (float)e.GetPosX());
		}
	}
}
