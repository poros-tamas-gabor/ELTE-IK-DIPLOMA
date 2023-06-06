#include "Keyboard.h"

Keyboard::Keyboard()
{
	for (int i = 0; i < 256u; i++)
	{
		this->m_keyStates[i] = false;
	}
}

bool Keyboard::KeyIsPressed(const unsigned char keycode) const
{
	return this->m_keyStates[keycode];

}
KeyboardEvent Keyboard::ReadKey()
{
	if (!this->m_keyBuffer.empty())
	{
		KeyboardEvent e = this->m_keyBuffer.front();
		this->m_keyBuffer.pop();
		return e;
	}
	else
		return KeyboardEvent();
}
unsigned char Keyboard::ReadChar()
{
	if (!this->m_charBuffer.empty())
	{
		unsigned char c = this->m_charBuffer.front();
		this->m_charBuffer.pop();
		return c;
	}
	else
		return 0u;
}

bool Keyboard::KeyBufferIsEmpty() const
{
	return this->m_keyBuffer.empty();
}
bool Keyboard::CharBufferIsEmpty() const
{
	return this->m_charBuffer.empty();
}

void Keyboard::ClearKeyStates()
{
	for (int i = 0; i < 256u; i++)
	{
		this->m_keyStates[i] = false;
	}
}
void Keyboard::ClearKeyBuffer()
{
	this->m_keyBuffer = std::queue<KeyboardEvent>();
}
void Keyboard::ClearCharBuffer()
{
	this->m_charBuffer = std::queue<unsigned char>();
}
void Keyboard::OnKeyPressed(const unsigned char key)
{
	this->m_keyStates[key] = true;
	this->m_keyBuffer.push(KeyboardEvent(KeyboardEvent::Press, key));
}
void Keyboard::OnKeyReleased(const unsigned char key)
{
	this->m_keyStates[key] = false;
	this->m_keyBuffer.push(KeyboardEvent(KeyboardEvent::Release, key));
}
void Keyboard::OnChar(const unsigned char key)
{
	this->m_charBuffer.push(key);
}
void Keyboard::EnableAutoRepeat()
{
	this->m_autoRepeat = true;
}
void Keyboard::DisableAutoRepeat()
{
	this->m_autoRepeat = false;
}
	 
bool Keyboard::IsAutoRepeatEnabled() const
{
	return this->m_autoRepeat;
}