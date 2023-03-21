#ifndef CONTROLLER_MESSAGE_SYSTEM
#define CONTROLLER_MESSAGE_SYSTEM

#include "IController.h"

#include <vector>
class MessageSystem
{
private:
	const std::vector<IControllerPtr>& m_subscriber;

public:
	void Publish(unsigned int message, float* fparam, unsigned* uparam);
	MessageSystem(const std::vector<IControllerPtr>& controllers);

	MessageSystem operator=(const MessageSystem&) = delete;
	MessageSystem(const MessageSystem&) = delete;
};
#endif // !CONTROLLER_MESSAGE_SYSTEM
