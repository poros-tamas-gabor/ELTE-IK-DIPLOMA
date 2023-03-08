#ifndef MOUSE_H
#define MOUSE_H
#include <queue>
#include "MouseEvent.h"
class Mouse 
{
private:
	std::queue<MouseEvent> _eventBuffer;
	int x = 0;
	int y = 0;
	bool leftIsDown = false;
	bool rightIsDown = false;
	bool mbuttonDown = false;

public:
	Mouse() = default;
	Mouse(const Mouse& other) = delete;
	Mouse& operator= (const Mouse& other) = delete;

	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseMoveRawRelative(int x, int y);
	void OnMouseMoveRawAbsolute(int x, int y);

	bool IsLeftDown() const;
	bool IsMiddleDown() const;
	bool IsRightDown() const;

	int GetPosX() const;
	int GetPosY() const;
	MousePoint GetPos() const;

	bool EventBufferIsEmpty() const;
	MouseEvent ReadEvent();

};
#endif
