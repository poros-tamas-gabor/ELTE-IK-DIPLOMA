#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "KeyboardEvent.h"
#include <mutex>
#include <queue>
class Keyboard
{
private:
	static Keyboard* _pinstance;
	static std::mutex _mutex;
	std::queue<KeyboardEvent> _keyBuffer;
	std::queue<unsigned char> _charBuffer;
	bool _autoRepeat = true;
	bool _keyStates[256];

private:
	Keyboard();
	~Keyboard() {}

public:

	static Keyboard* GetInstance(void);
	static void Shutdown(void);

	Keyboard(const Keyboard& other) = delete;
	Keyboard& operator= (const Keyboard& other) = delete;

	bool KeyIsPressed(const unsigned char keycode) const;
	KeyboardEvent ReadKey();
	unsigned char ReadChar();

	bool KeyBufferIsEmpty() const;
	bool CharBufferIsEmpty() const;

	void ClearKeyBuffer();
	void ClearCharBuffer();

	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);

	void EnableAutoRepeat();
	void DisableAutoRepeat();


	bool IsAutoRepeatEnabled() const;
};
#endif