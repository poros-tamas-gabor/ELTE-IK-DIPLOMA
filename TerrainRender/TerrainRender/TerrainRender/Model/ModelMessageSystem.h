#ifndef MODEL_MESSAGE_SYSTEM
#define MODEL_MESSAGE_SYSTEM

#include "../View/IView.h"
#include "Persistence/ModelStructs.h"
#include <vector>
class ModelMessageSystem
{
private:
	std::vector<IViewPtr> m_subscriber;

public:
	ModelMessageSystem() = default;
	void PublishModelState(const std::vector<IRenderableState>&);
	void PublishModelState(const FlythroughState&);
	void PublishModelState(const Explore3DState&);
	bool Subscribe(IViewPtr view);
	bool Unsubscribe(IViewPtr view);

	ModelMessageSystem operator=(const ModelMessageSystem&) = delete;
	ModelMessageSystem(const ModelMessageSystem&) = delete;
};
#endif // !CONTROLLER_MESSAGE_SYSTEM