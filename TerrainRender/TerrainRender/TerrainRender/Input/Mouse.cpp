#include "Mouse.h"

void Mouse::OnLeftPressed(int x, int y)
{
	this->leftIsDown = true;
	this->m_eventBuffer.push(MouseEvent(MouseEvent::LPress, x, y));
}
void Mouse::OnLeftReleased(int x, int y)
{
	this->leftIsDown = false;
	this->m_eventBuffer.push(MouseEvent(MouseEvent::LRelease, x, y));
}
void Mouse::OnRightPressed(int x, int y)
{
	this->rightIsDown = true;
	this->m_eventBuffer.push(MouseEvent(MouseEvent::RPress, x, y));
}
void Mouse::OnRightReleased(int x, int y)
{
	this->rightIsDown = false;
	this->m_eventBuffer.push(MouseEvent(MouseEvent::RRelease, x, y));
}
void Mouse::OnMiddlePressed(int x, int y)
{
	this->mbuttonDown = true;
	this->m_eventBuffer.push(MouseEvent(MouseEvent::MPress, x, y));
}
void Mouse::OnMiddleReleased(int x, int y)
{
	this->mbuttonDown = false;
	this->m_eventBuffer.push(MouseEvent(MouseEvent::MRelease, x, y));
}
void Mouse::OnWheelUp(int x, int y)
{
	this->m_eventBuffer.push(MouseEvent(MouseEvent::WheelUp, x, y));
}
void Mouse::OnWheelDown(int x, int y)
{
	this->m_eventBuffer.push(MouseEvent(MouseEvent::WheelDown, x, y));
}
void Mouse::OnMouseMove(int x, int y) 
{
	this->x = x;
	this->y = y;
	this->m_eventBuffer.push(MouseEvent(MouseEvent::Move, x, y));
}
void Mouse::OnMouseMoveRawRelative(int x, int y)
{
	this->m_eventBuffer.push(MouseEvent(MouseEvent::RAW_MOVE_RELATIVE, x, y));
}

void Mouse::OnMouseMoveRawAbsolute(int x, int y)
{
	this->m_eventBuffer.push(MouseEvent(MouseEvent::RAW_MOVE_ABSOLUTE, x, y));
}
	 
bool Mouse::IsLeftDown() const
{
	return this->leftIsDown;
}
bool Mouse::IsMiddleDown() const
{
	return this->mbuttonDown;
}
bool Mouse::IsRightDown() const
{
	return this->rightIsDown;
}

int Mouse::GetPosX() const
{
	return this->x;
}
int Mouse::GetPosY() const
{
	return this->y;
}
MousePoint Mouse::GetPos() const
{
	return { this->x, this->y };
}

bool Mouse::EventBufferIsEmpty() const
{
	return this->m_eventBuffer.empty();
}
MouseEvent Mouse::ReadEvent()
{
	if (!m_eventBuffer.empty())
	{
		MouseEvent e = this->m_eventBuffer.front();
		this->m_eventBuffer.pop();
		return e;
	}
	return MouseEvent();
}

void Mouse::ClearMouseEventBuffer(void)
{
	this->m_eventBuffer = std::queue<MouseEvent>();
}
void Mouse::ClearMouseState(void)
{
	this->leftIsDown = false;
	this->mbuttonDown = false;
	this->rightIsDown = false;
}