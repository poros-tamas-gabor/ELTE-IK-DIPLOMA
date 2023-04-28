#include "MessageSystem.h"

void ControllerMessageSystem::Publish(IControllerMessageIDs message, const std::vector<float>& fparam, const std::vector<unsigned>& uparam)
{
	for (IControllerPtr controller : m_subscribers)
	{
		if(controller->CanHandle(message))
			controller->HandleMessage(message, fparam, uparam);
	}
}
ControllerMessageSystem::ControllerMessageSystem(const std::vector<IControllerPtr>& controllers) : m_subscribers(controllers) {}

