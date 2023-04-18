#include "MessageSystem.h"

void ControllerMessageSystem::Publish(unsigned int message, float* fparam, unsigned* uparam)
{
	for (IControllerPtr controller : m_subscribers)
	{
		if(controller->CanHandle(message))
			controller->HandleMessage(message, fparam, uparam);
	}
}
ControllerMessageSystem::ControllerMessageSystem(const std::vector<IControllerPtr>& controllers) : m_subscribers(controllers) {}

