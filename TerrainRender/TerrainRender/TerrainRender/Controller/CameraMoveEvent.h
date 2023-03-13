#ifndef CAMERA_MOVE_EVENT_H
#define CAMERA_MOVE_EVENT_H

#include <string>
#include "../EventTemplate.h"

enum CameraMoveEvent_Type
{
	Invalid,
	MoveForward,
	MoveBack,
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown

};
//Data == TimeElapsed unsigned int
typedef Event<CameraMoveEvent_Type, unsigned int> CameraMoveEvent;
#endif
