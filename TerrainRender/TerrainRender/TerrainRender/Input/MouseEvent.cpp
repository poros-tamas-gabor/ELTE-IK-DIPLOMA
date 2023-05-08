#include "MouseEvent.h"

MouseEvent::MouseEvent() : m_type(MouseEvent::Type::Invalid), m_mousepoint({0,0}) {}
MouseEvent::MouseEvent(MouseEvent::Type type, int x, int y) : m_type(type), m_mousepoint({ x,y }) {}

bool MouseEvent::IsValid() const
{
	return ! (this->m_type == MouseEvent::Type::Invalid);
}
MouseEvent::Type MouseEvent::GetType() const
{
	return this->m_type;
}
MousePoint MouseEvent::GetPos() const
{
	return this->m_mousepoint;
}
int MouseEvent::GetPosX() const
{
	return this->m_mousepoint.x;
}
int MouseEvent::GetPosY() const
{
	return this->m_mousepoint.y;

}
