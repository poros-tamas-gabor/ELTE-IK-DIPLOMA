#include "ModelMessageSystem.h"

void ModelMessageSystem::PublishRenderableInformation(const std::vector<IRenderableInformation>& IRenderableinfo)
{
	for (IViewPtr view : m_subscriber)
	{
		view->HandleIRenderableInfo(IRenderableinfo);
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