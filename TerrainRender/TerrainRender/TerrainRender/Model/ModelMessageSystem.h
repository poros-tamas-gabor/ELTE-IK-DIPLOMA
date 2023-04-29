#ifndef MODEL_MESSAGE_SYSTEM
#define MODEL_MESSAGE_SYSTEM

#include "IModelSubscriber.h"
#include "Persistence/ModelStructs.h"
#include <vector>
class ModelMessageSystem
{
private:
	std::vector<IModelSubscriberPtr> m_subscribers;

public:
	ModelMessageSystem() = default;
	void PublishModelState(const MeshGroupState&) const;
	void PublishModelState(const FlythroughState&) const;
	void PublishModelState(const Explore3DState&) const;
	void PublishModelState(const CameraState&) const;
	bool Subscribe(IModelSubscriberPtr view);
	bool Unsubscribe(IModelSubscriberPtr view);

	ModelMessageSystem operator=(const ModelMessageSystem&) = delete;
	ModelMessageSystem(const ModelMessageSystem&) = delete;
};
#endif // !CONTROLLER_MESSAGE_SYSTEM