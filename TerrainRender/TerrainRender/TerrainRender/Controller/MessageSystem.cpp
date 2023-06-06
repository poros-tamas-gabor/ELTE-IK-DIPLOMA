#include "MessageSystem.h"

bool ControllerMessageSystem::Publish(IControllerMessageIDs message, const std::vector<float>& fparam, const std::vector<unsigned>& uparam)
{
	bool success = true;
	for (IControllerPtr controller : m_subscribers)
	{
		success &= controller->HandleMessage(message, fparam, uparam);
	}
	return success;
}
ControllerMessageSystem::ControllerMessageSystem(const std::vector<IControllerPtr>& controllers) : m_subscribers(controllers) {}

