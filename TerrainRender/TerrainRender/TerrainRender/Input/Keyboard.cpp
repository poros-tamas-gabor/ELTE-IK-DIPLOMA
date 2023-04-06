#include "Keyboard.h"

Keyboard::Keyboard()
{
	for (int i = 0; i < 256u; i++)
	{
		this->_keyStates[i] = false;
	}
}

bool Keyboard::KeyIsPressed(const unsigned char keycode) const
{
	return this->_keyStates[keycode];

}
KeyboardEvent Keyboard::ReadKey()
{
	if (!this->_keyBuffer.empty())
	{
		KeyboardEvent e = this->_keyBuffer.front();
		this->_keyBuffer.pop();
		return e;
	}
	else
		return KeyboardEvent();
}
unsigned char Keyboard::ReadChar()
{
	if (!this->_charBuffer.empty())
	{
		unsigned char c = this->_charBuffer.front();
		this->_charBuffer.pop();
		return c;
	}
	else
		return 0u;
}

bool Keyboard::KeyBufferIsEmpty() const
{
	return this->_keyBuffer.empty();
}
bool Keyboard::CharBufferIsEmpty() const
{
	return this->_charBuffer.empty();
}

void Keyboard::ClearKeyStates()
{
	for (int i = 0; i < 256u; i++)
	{
		this->_keyStates[i] = false;
	}
}
void Keyboard::ClearKeyBuffer()
{
	this->_keyBuffer = std::queue<KeyboardEvent>();
}
void Keyboard::ClearCharBuffer()
{
	this->_charBuffer = std::queue<unsigned char>();
}
void Keyboard::OnKeyPressed(const unsigned char key)
{
	this->_keyStates[key] = true;
	this->_keyBuffer.push(KeyboardEvent(KeyboardEvent::Press, key));
}
void Keyboard::OnKeyReleased(const unsigned char key)
{
	this->_keyStates[key] = false;
	this->_keyBuffer.push(KeyboardEvent(KeyboardEvent::Release, key));
}
void Keyboard::OnChar(const unsigned char key)
{
	this->_charBuffer.push(key);
}
void Keyboard::EnableAutoRepeat()
{
	this->_autoRepeat = true;
}
void Keyboard::DisableAutoRepeat()
{
	this->_autoRepeat = false;
}
	 
bool Keyboard::IsAutoRepeatEnabled() const
{
	return this->_autoRepeat;
}