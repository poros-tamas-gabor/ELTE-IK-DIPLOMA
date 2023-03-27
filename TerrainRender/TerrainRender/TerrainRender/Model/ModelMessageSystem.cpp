#include "ModelMessageSystem.h"

void ModelMessageSystem::PublishModelState(const std::vector<IRenderableState>& IRenderableinfo)
{
	for (IViewPtr view : m_subscriber)
	{
		view->HandleIModelState(IRenderableinfo);
	}
}
void ModelMessageSystem::PublishModelState(const FlythroughState& state)
{
	for (IViewPtr view : m_subscriber)
	{
		view->HandleIModelState(state);
	}
}
void ModelMessageSystem::PublishModelState(const Explore3DState& state)
{
	for (IViewPtr view : m_subscriber)
	{
		view->HandleIModelState(state);
	}
}
bool ModelMessageSystem::Subscribe(IViewPtr view)
{
	if (std::find(this->m_subscriber.begin(), this->m_subscriber.end(), view) != this->m_subscriber.end())
	{
		return false;
	}
	else {
		this->m_subscriber.push_back(view);
		return true;
	}
}
bool ModelMessageSystem::Unsubscribe(IViewPtr view)
{
	if (std::find(this->m_subscriber.begin(), this->m_subscriber.end(), view) != this->m_subscriber.end())
	{
		std::remove(this->m_subscriber.begin(), this->m_subscriber.end(), view);
		return true;
	}
	return false;
}