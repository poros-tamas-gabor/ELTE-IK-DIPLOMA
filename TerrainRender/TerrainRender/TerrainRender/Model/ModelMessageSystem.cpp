#include "ModelMessageSystem.h"
#include "../resource.h"

void ModelMessageSystem::PublishModelState(const MeshGroupState& state) const
{
	for (IModelSubscriberPtr subscriber : m_subscribers)
	{
		subscriber->HandleIModelState(state);
	}
}
void ModelMessageSystem::PublishModelState(const FlythroughState& state) const
{
	for (IModelSubscriberPtr subscriber : m_subscribers)
	{
		subscriber->HandleIModelState(state);
	}
}
void ModelMessageSystem::PublishModelState(const Explore3DState& state) const
{
	for (IModelSubscriberPtr subscriber : m_subscribers)
	{
		subscriber->HandleIModelState(state);
	}
}

void ModelMessageSystem::PublishModelState(const CameraState& state) const
{
	for (IModelSubscriberPtr subscriber : m_subscribers)
	{
		subscriber->HandleIModelState(state);
	}
}

bool ModelMessageSystem::Subscribe(IModelSubscriberPtr subscriber)
{
	if (std::find(this->m_subscribers.begin(), this->m_subscribers.end(), subscriber) != this->m_subscribers.end())
	{
		return false;
	}
	else {
		this->m_subscribers.push_back(subscriber);
		return true;
	}
}
bool ModelMessageSystem::Unsubscribe(IModelSubscriberPtr subscriber)
{
	if (std::find(this->m_subscribers.begin(), this->m_subscribers.end(), subscriber) != this->m_subscribers.end())
	{
		std::remove(this->m_subscribers.begin(), this->m_subscribers.end(), subscriber);
		return true;
	}
	return false;
}
