#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent() : _type (KeyboardEvent::Type::Invalid) , _code(0u) {}
KeyboardEvent::KeyboardEvent(KeyboardEvent::Type type, unsigned char code) : _type(type), _code(code) {}

bool KeyboardEvent::IsPress() const
{
	return this->_type == KeyboardEvent::Type::Press;
}
bool KeyboardEvent::IsRelease() const
{
	return this->_type == KeyboardEvent::Type::Release;

}
bool KeyboardEvent::IsValid() const
{
	return this->_type != KeyboardEvent::Type::Invalid;
}
unsigned char KeyboardEvent::GetKeyCode() const
{
	return this->_code;
}