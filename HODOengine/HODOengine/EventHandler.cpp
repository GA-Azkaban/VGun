#include "EventHandler.h"
#include "RendererBase.h"

namespace HDData
{
	std::unordered_map<GameObject*, std::unordered_set<EventHandler*>> EventHandler::_eventHandlers;

	EventHandler::EventHandler()
	{
		_eventHandlers[GetGameObject()].insert(this);
	}

	EventHandler::~EventHandler()
	{
		_eventHandlers[GetGameObject()].erase(this);
	}

	void EventHandler::CallEvents(HDEngine::IRenderable* renderable, std::function<void(EventHandler*)> todo)
	{
		if (renderable == nullptr)
			return;

		auto renderableIter = RendererBase::_renderableMap.find(renderable);
		if (renderableIter == RendererBase::_renderableMap.end())
			return;
		RendererBase* renderer = renderableIter->second;

		GameObject* gameObject = renderer->GetGameObject();
		for (auto& eachHandler : _eventHandlers[gameObject])
		{
			todo(eachHandler);
		}
	}
}
