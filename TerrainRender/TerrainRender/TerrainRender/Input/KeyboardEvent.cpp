#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent() : m_type (KeyboardEvent::Type::Invalid) , m_code(0u) {}
KeyboardEvent::KeyboardEvent(KeyboardEvent::Type type, unsigned char code) : m_type(type), m_code(code) {}

bool KeyboardEvent::IsPress() const
{
	return this->m_type == KeyboardEvent::Type::Press;
}
bool KeyboardEvent::IsRelease() const
{
	return this->m_type == KeyboardEvent::Type::Release;

}
bool KeyboardEvent::IsValid() const
{
	return this->m_type != KeyboardEvent::Type::Invalid;
}
unsigned char KeyboardEvent::GetKeyCode() const
{
	return this->m_code;
}