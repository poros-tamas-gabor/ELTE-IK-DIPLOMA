#ifndef CAMERA_MOVE_EVENT_H
#define CAMERA_MOVE_EVENT_H

#include <string>
#include "../EventTemplate.h"

namespace CameraMoveEvent
{
enum Type
{
	Invalid,
	MoveForward,
	MoveBack,
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown
};
typedef ::Event<Type, float> Event;
}
//Data == TimeElapsed unsigned int
#endif
