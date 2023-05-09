#ifndef MODEL_MESSAGE_SYSTEM
#define MODEL_MESSAGE_SYSTEM

///////////////////////////////////////////////////////////////////////////////
// ModelMessageSystem.h
// ====================
// 
// The communication between the model and the observers is implemented according to the Observer design pattern.
// The IModelSubscriber interface specifies the HandleModelState function, which is capable of receiving various complex data structures. 
// These structures store, for example, the current state of the model in the view and display it on the user interface.
//
// The Observer design pattern is provided by the ModelMessageSystem class.
// Observers of type IModelSubscriber can subscribe to this object.
// When the state of the IModel object changes, the ModelMessageSystem object notifies the subscribers through the PublishModelState method.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

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
	void PublishModelState(const GeneralModelState&) const;
	bool Subscribe(IModelSubscriberPtr view);
	bool Unsubscribe(IModelSubscriberPtr view);

	ModelMessageSystem operator=(const ModelMessageSystem&) = delete;
	ModelMessageSystem(const ModelMessageSystem&) = delete;
};
#endif // !MODEL_MESSAGE_SYSTEM