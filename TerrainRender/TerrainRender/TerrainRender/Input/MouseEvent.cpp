#include "MouseEvent.h"

MouseEvent::MouseEvent() : _type(MouseEvent::Type::Invalid), _mousepoint({0,0}) {}
MouseEvent::MouseEvent(MouseEvent::Type type, int x, int y) : _type(type), _mousepoint({ x,y }) {}

bool MouseEvent::IsValid() const
{
	return ! (this->_type == MouseEvent::Type::Invalid);
}
MouseEvent::Type MouseEvent::GetType() const
{
	return this->_type;
}
MousePoint MouseEvent::GetPos() const
{
	return this->_mousepoint;
}
int MouseEvent::GetPosX() const
{
	return this->_mousepoint.x;
}
int MouseEvent::GetPosY() const
{
	return this->_mousepoint.y;

}
