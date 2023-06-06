#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

///////////////////////////////////////////////////////////////////////////////
// MouseEvent.h
// ============
// A class that encapsulates a mouse event, providing information about the event type and the state of the mouse when the event occurred.
// The MouseEvent class is used by the Mouse class.
//
// The possible types of mouse events are defined by the MouseEvent::EventType enumeration.
//
// To use this class, it must be instantiated with a MouseEvent::EventType.
//
// AUTHOR: Pindrought
// URL: https://github.com/Pindrought/DirectX-11-Engine-VS2017
// CREATED: Unknown
// MODIFIED: 2023-05-08 by TAMAS GABOR POROS
///////////////////////////////////////////////////////////////////////////////

struct MousePoint
{
	int x;
	int y;
};
class MouseEvent
{
public:
	enum Type
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RAW_MOVE_ABSOLUTE,
		RAW_MOVE_RELATIVE,
		Invalid
	};
private:
	MouseEvent::Type m_type;
	MousePoint m_mousepoint;

public:
	MouseEvent();
	MouseEvent(MouseEvent::Type type, int x, int y);

	bool IsValid() const;
	MouseEvent::Type GetType() const;
	MousePoint GetPos() const;
	int GetPosX() const;
	int GetPosY() const;

};
#endif