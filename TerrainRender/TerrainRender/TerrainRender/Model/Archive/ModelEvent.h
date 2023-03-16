#ifndef MDEL_EVENT_H
#define MDEL_EVENT_H

#include <string>
#include "../EventTemplate.h"
#include "Persistence/ModelStructs.h"
namespace ModelEvent
{
	enum Type
	{
		Invalid,
		TerrainLoaded,
		CameraTrajectoryLoaded,
		CameraPropertiesLoaded
	};
	typedef ::Event<Type, const std::vector<ModelVertex>&> Event;
}
//Data == TimeElapsed unsigned int
#endif
