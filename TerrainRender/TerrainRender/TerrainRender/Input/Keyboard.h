#ifndef KEYBOARD_H
#define KEYBOARD_H

///////////////////////////////////////////////////////////////////////////////
// Keyboard.h
// ===============
// A class that encapsulates the functionality of the keyboard input device. 
// The class maintains the current state of each key on the keyboard, which can be accessed using the various public methods.
//
// This class can also generate KeyboardEvents when keys are pressed or released. These events can be retrieved by calling the ReadKey or ReadChar method.
//
// AUTHOR: Pindrought
// URL: https://github.com/Pindrought/DirectX-11-Engine-VS2017
// CREATED: Unknown
// MODIFIED: 2023-05-08 by TAMAS GABOR POROS
///////////////////////////////////////////////////////////////////////////////

#include "KeyboardEvent.h"
#include <mutex>
#include <queue>
#include <memory>
class Keyboard
{
private:
	std::queue<KeyboardEvent> m_keyBuffer;
	std::queue<unsigned char> m_charBuffer;
	bool m_autoRepeat = true;
	bool m_keyStates[256];

private:

public:
	Keyboard();
	~Keyboard() {}

	Keyboard(const Keyboard& other) = delete;
	Keyboard& operator= (const Keyboard& other) = delete;

	bool KeyIsPressed(const unsigned char keycode) const;
	KeyboardEvent ReadKey();
	unsigned char ReadChar();

	bool KeyBufferIsEmpty() const;
	bool CharBufferIsEmpty() const;

	void ClearKeyStates();
	void ClearKeyBuffer();
	void ClearCharBuffer();

	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);

	void EnableAutoRepeat();
	void DisableAutoRepeat();

	bool IsAutoRepeatEnabled() const;
};

typedef std::shared_ptr<Keyboard> KeyboardPtr;
#endif