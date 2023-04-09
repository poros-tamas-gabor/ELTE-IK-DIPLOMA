#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "KeyboardEvent.h"
#include <mutex>
#include <queue>
#include <memory>
class Keyboard
{
private:
	std::queue<KeyboardEvent> _keyBuffer;
	std::queue<unsigned char> _charBuffer;
	bool _autoRepeat = true;
	bool _keyStates[256];

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