#include "Controllers.h"
#include "../Graphics/Graphics.h"

void Controller3DExplore::Control(float dt, Graphics* graphics)
{
	while (!Keyboard::GetInstance()->KeyBufferIsEmpty())
	{
		KeyboardEvent e = Keyboard::GetInstance()->ReadKey();
	}
	while (!Keyboard::GetInstance()->CharBufferIsEmpty())
	{
		unsigned char c = Keyboard::GetInstance()->ReadChar();
	}

	while (!Mouse::GetInstance()->EventBufferIsEmpty())
	{
		MouseEvent e = Mouse::GetInstance()->ReadEvent();
		ControlMouse(e, graphics);
	}


	if (Keyboard::GetInstance()->KeyIsPressed('W'))
	{
		graphics->_position.MoveForward(dt);
	}
	if (Keyboard::GetInstance()->KeyIsPressed('S'))
	{
		graphics->_position.MoveBack(dt);
	}
	if (Keyboard::GetInstance()->KeyIsPressed('A'))
	{
		graphics->_position.MoveLeft(dt);
	}
	if (Keyboard::GetInstance()->KeyIsPressed('D'))
	{
		graphics->_position.MoveRight(dt);
	}
}

void Controller3DExplore::ControlMouse(const MouseEvent& e, Graphics* graphics)
{
	if (MouseEvent::Type::RAW_MOVE_ABSOLUTE == e.GetType())
	{
		static MouseEvent prev;
		if (prev.IsValid())
		{
			int yaw = e.GetPosX() - prev.GetPosX();
			int pitch = e.GetPosY() - prev.GetPosY();

			graphics->_position.RotatePitchYaw((float)pitch, (float)yaw);

		}
		prev = e;
	}
	else if (MouseEvent::Type::RAW_MOVE_RELATIVE == e.GetType())
	{
		graphics->_position.RotatePitchYaw((float)e.GetPosY(), (float)e.GetPosX());
	}
}

void Controller3DExplore::ControlKeyboard(const KeyboardEvent& e, float dt, Graphics* graphics)
{
	if (e.IsPress())
	{
		switch (e.GetKeyCode())
		{
		case 'W':
		{
			graphics->_position.MoveForward(dt);
			break;
		}
		case 'A':
		{
			graphics->_position.MoveLeft(dt);
			break;
		}
		case 'S':
		{
			graphics->_position.MoveBack(dt);
			break;
		}
		case 'D':
		{
			graphics->_position.MoveRight(dt);
			break;
		}
		default:
			break;
		}

	}
}