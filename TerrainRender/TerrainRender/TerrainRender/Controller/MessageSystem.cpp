#include "MessageSystem.h"

void MessageSystem::Publish(unsigned int message, float* fparam, unsigned* uparam)
{
	for (IControllerPtr controller : m_subscriber)
	{
		if(controller->CanHandle(message))
			controller->HandleMessage(message, fparam, uparam);
	}
}
MessageSystem::MessageSystem(const std::vector<IControllerPtr>& controllers) : m_subscriber(controllers) {}

