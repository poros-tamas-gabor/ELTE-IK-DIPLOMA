#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

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
	MouseEvent::Type _type;
	MousePoint _mousepoint;

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