#include "ModelMessageSystem.h"

void ModelMessageSystem::PublishModelState(const std::vector<IRenderableState>& IRenderableinfo) const
{
	for (IViewPtr view : m_subscriber)
	{
		view->HandleIModelState(IRenderableinfo);
	}
}
void ModelMessageSystem::PublishModelState(const FlythroughState& state) const
{
	for (IViewPtr view : m_subscriber)
	{
		view->HandleIModelState(state);
	}
}
void ModelMessageSystem::PublishModelState(const Explore3DState& state) const
{
	for (IViewPtr view : m_subscriber)
	{
		view->HandleIModelState(state);
	}
}

void ModelMessageSystem::PublishModelState(const ResizedWindowState& state) const
{
	for (IControllerPtr controller : m_controllers)
	{
		unsigned params[2] = { state.screenWidth, state.screenHeight };
		controller->HandleMessage(WM_SIZE, NULL, params);
	}
}

bool ModelMessageSystem::Subscribe(IControllerPtr controller)
{
	if (std::find(this->m_controllers.begin(), this->m_controllers.end(), controller) != this->m_controllers.end())
	{
		return false;
	}
	else {
		this->m_controllers.push_back(controller);
		return true;
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

bool ModelMessageSystem::Unsubscribe(IControllerPtr view)
{
	if (std::find(this->m_controllers.begin(), this->m_controllers.end(), view) != this->m_controllers.end())
	{
		std::remove(this->m_controllers.begin(), this->m_controllers.end(), view);
		return true;
	}
	return false;
}