#ifndef MODEL_MESSAGE_SYSTEM
#define MODEL_MESSAGE_SYSTEM

#include "../View/IView.h"
#include "../Controller/IController.h"
#include "Persistence/ModelStructs.h"
#include <vector>
class ModelMessageSystem
{
private:
	std::vector<IViewPtr> m_subscriber;
	std::vector<IControllerPtr> m_controllers;

public:
	ModelMessageSystem() = default;
	void PublishModelState(const std::vector<IRenderableState>&) const;
	void PublishModelState(const FlythroughState&) const;
	void PublishModelState(const Explore3DState&) const;
	void PublishModelState(const ResizedWindowState&) const;
	bool Subscribe(IViewPtr view);
	bool Subscribe(IControllerPtr view);
	bool Unsubscribe(IViewPtr view);
	bool Unsubscribe(IControllerPtr view);

	ModelMessageSystem operator=(const ModelMessageSystem&) = delete;
	ModelMessageSystem(const ModelMessageSystem&) = delete;
};
#endif // !CONTROLLER_MESSAGE_SYSTEM