#ifndef MODEL_I_EVENT_H
#define MODEL_I_EVENT_H

#include <string>

namespace ModelEvent
{
	enum Type
	{
		Invalid,
		SetCameraMoveSpeed,
		SetCameraRotationSpeed,
		ResetCamera

	};

	class IEvent {
	public:
		virtual ~IEvent() {}
		virtual Type GetEventType() = 0;
		virtual void SetEventType(Type) = 0;
	};
}

#endif
