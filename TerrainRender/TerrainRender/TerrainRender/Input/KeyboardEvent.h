#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H


class KeyboardEvent
{
public:
	enum Type
	{
		Press,
		Release,
		Invalid
	};
private:
	KeyboardEvent::Type _type;
	unsigned char _code;

public:
	KeyboardEvent();
	KeyboardEvent(KeyboardEvent::Type type, unsigned char code);

	bool IsPress() const;
	bool IsRelease() const;
	bool IsValid() const;
	unsigned char GetKeyCode() const;

};
#endif