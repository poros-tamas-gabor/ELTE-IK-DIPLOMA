#ifndef I_EVENT_H
#define I_EVENT_H

#include <string>

namespace ControllerEvent
{
	enum Type
	{
		Invalid,
		NewFrame,
		ClickButton_SetTerrainPath,
		ClickButton_SetTrajectoryPath,
		ClickButton_SetCameraPropertiesPath,
		FlipMode
		
	};

	class IEvent {
	public:
		virtual ~IEvent() {}
		virtual ControllerEvent::Type GetEventType() = 0;
	};
}



#endif