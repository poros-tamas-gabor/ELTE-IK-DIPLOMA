#include "Controllers.h"
#include "../Graphics/Graphics.h"

void Controller3DExplore::Control(float dt, Graphics* graphics) const
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

	if (Keyboard::GetInstance()->KeyIsPressed(VK_SPACE))
	{
		graphics->_position.MoveUp(dt);
	}
	if (Keyboard::GetInstance()->KeyIsPressed('C'))
	{
		graphics->_position.MoveDown(dt);
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

void Controller3DExplore::ControlMouse(const MouseEvent& e, Graphics* graphics) const
{
	if (MouseEvent::Type::RAW_MOVE_ABSOLUTE == e.GetType())
	{
		static MouseEvent prev;
		if (prev.IsValid())
		{
			int yaw = e.GetPosX() - prev.GetPosX();
			int pitch = e.GetPosY() - prev.GetPosY();

			if (Mouse::GetInstance()->IsLeftDown())
			{
				graphics->_position.RotatePitchYaw((float)pitch, (float)yaw);
			}

		}
		prev = e;
	}
	else if (MouseEvent::Type::RAW_MOVE_RELATIVE == e.GetType())
	{
		if (Mouse::GetInstance()->IsLeftDown())
		{
			graphics->_position.RotatePitchYaw((float)e.GetPosY(), (float)e.GetPosX());
		}
	}
}
