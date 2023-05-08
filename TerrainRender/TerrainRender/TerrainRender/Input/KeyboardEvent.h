#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H

///////////////////////////////////////////////////////////////////////////////
// KeyboardEvent.h
// ===============
// A class that encapsulates information about a keyboard event, such as whether a key was pressed or released and which key was affected.
//
// To use this class, create a KeyboardEvent object and pass it to the appropriate event handler function.
// The event handler function can then extract information about the keyboard event from the KeyboardEvent object.
//
// AUTHOR: Pindrought
// URL: https://github.com/Pindrought/DirectX-11-Engine-VS2017
// CREATED: Unknown
// MODIFIED: 2023-05-08 by TAMAS GABOR POROS
///////////////////////////////////////////////////////////////////////////////

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
	KeyboardEvent::Type m_type;
	unsigned char m_code;

public:
	KeyboardEvent();
	KeyboardEvent(KeyboardEvent::Type type, unsigned char code);

	bool IsPress() const;
	bool IsRelease() const;
	bool IsValid() const;
	unsigned char GetKeyCode() const;

};
#endif