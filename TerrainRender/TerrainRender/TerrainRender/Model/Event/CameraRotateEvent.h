#ifndef CAMERA_ROTATE_EVENT_H
#define CAMERA_ROTATE_EVENT_H

#include <string>
#include "EventTemplate.h"

namespace CameraRotateEvent
{
	enum Type
	{
		Invalid,
		Rotate
	};

	struct CameraRotation
	{
		float pitch;
		float yaw;
	};

	typedef ::Event<Type, CameraRotation> Event;
}

#endif
