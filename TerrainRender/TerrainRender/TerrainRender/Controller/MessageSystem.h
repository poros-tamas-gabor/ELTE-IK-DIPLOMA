#ifndef CONTROLLER_MESSAGE_SYSTEM
#define CONTROLLER_MESSAGE_SYSTEM

#include "IController.h"
#include <vector>
#include <memory>
class ControllerMessageSystem
{
private:
	const std::vector<IControllerPtr>& m_subscribers;

public:
	void Publish(unsigned int message, float* fparam, unsigned* uparam);
	ControllerMessageSystem(const std::vector<IControllerPtr>& controllers);

	ControllerMessageSystem operator=(const ControllerMessageSystem&) = delete;
	ControllerMessageSystem(const ControllerMessageSystem&) = delete;
};

typedef std::shared_ptr<ControllerMessageSystem> ControllerMessageSystemPtr;
#endif // !CONTROLLER_MESSAGE_SYSTEM
