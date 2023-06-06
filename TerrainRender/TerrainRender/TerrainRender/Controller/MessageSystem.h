#ifndef CONTROLLER_MESSAGE_SYSTEM
#define CONTROLLER_MESSAGE_SYSTEM

///////////////////////////////////////////////////////////////////////////////
// MessageSystem.h
// ===============
// A class that facilitates communication between controllers using the Observer pattern.
//
// Each controller has a reference to a MessageSystem object, and can use the MessageSystem::Publish method to send messages to other subscribed controllers.
// The subscribed controllers receive the message via their IController::HandleMassage method, which they need to implement themselves.
//
// To use this class, each controller must have a reference to a MessageSystem object, which is created by the ControllerRouter.
// The ControllerRouter is also responsible for adding the other controllers as subscribers to the MessageSystem via the AddController method.
// Once subscribed, the controller can receive messages from other subscribed controllers and act accordingly by implementing their own HandleMessage method.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include "IController.h"
#include <vector>
#include <memory>
class ControllerMessageSystem
{
private:
	const std::vector<IControllerPtr>& m_subscribers;

public:
	ControllerMessageSystem(const std::vector<IControllerPtr>& controllers);
	ControllerMessageSystem operator=(const ControllerMessageSystem&) = delete;
	ControllerMessageSystem(const ControllerMessageSystem&) = delete;

	bool Publish(IControllerMessageIDs message, const std::vector<float>& fparam, const std::vector<unsigned>& uparam);
};

typedef std::shared_ptr<ControllerMessageSystem> ControllerMessageSystemPtr;
#endif // !CONTROLLER_MESSAGE_SYSTEM
