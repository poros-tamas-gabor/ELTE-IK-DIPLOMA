#ifndef INPUT_H
#define INPUT_H

#include "Keyboard.h"
#include "Mouse.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
class Input
{
public:
	Keyboard	_keyboard;
	Mouse		_mouse;

	Input() = default;
	Input(const Input &other) = delete;
	Input& operator= (const Input &other) = delete;

	LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

};

#endif