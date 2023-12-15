#include "EventHandler.h"

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
		//GameObject* gameObject = renderable.GetGameObject
		for (auto& eachHandler : _eventHandlers[gameObject])
		{
			todo(eachHandler);
		}
	}
}
