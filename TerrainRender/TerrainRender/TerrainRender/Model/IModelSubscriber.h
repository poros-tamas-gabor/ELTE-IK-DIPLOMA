#ifndef I_MODEL_SUBSCRIBER_H
#define I_MODEL_SUBSCRIBER_H
#include "IModel.h"

class IModelSubscriber
{
public:
	virtual ~IModelSubscriber() = default;
	virtual void HandleIModelState(const MeshGroupState&) = 0;
	virtual void HandleIModelState(const FlythroughState&) = 0;
	virtual void HandleIModelState(const Explore3DState&) = 0;
	virtual void HandleIModelState(const CameraState&) = 0;
};

typedef std::shared_ptr<IModelSubscriber> IModelSubscriberPtr;
#endif
