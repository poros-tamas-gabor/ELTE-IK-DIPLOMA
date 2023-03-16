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
		FlipMode,

		EnterFloat_cameraMoveSpeed,
		EnterFloat_cameraRotationSpeed,

		ClickButton_ResetCamera
		
	};

	class IEvent {
	public:
		virtual ~IEvent() {}
		virtual ControllerEvent::Type GetEventType() = 0;
		virtual void SetEventType(ControllerEvent::Type) = 0;
	};
}



#endif