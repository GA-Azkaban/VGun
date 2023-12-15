#pragma once
#include "Component.h"
#include <functional>
#include <unordered_map>
#include <unordered_set>
/// <summary>
/// ClickEvent와 HoverEvent를 등록하고 호출한다.
/// 2023.12.15 MJKIM
/// </summary>

namespace HDEngine
{
	class IRenderable;
}

namespace HDData
{
	class GameObject;

	class EventHandler : public Component
	{
	public:
		EventHandler();
		~EventHandler();

		static void CallEvents(HDEngine::IRenderable* renderable, std::function<void(EventHandler*)> todo);
		void SetClickEvent(std::function<void()> clickEvent);
		void SetHoverEvent(std::function<void()> hoverEvent);

	private:
		static std::unordered_map<GameObject*, std::unordered_set<EventHandler*>> _eventHandlers;
		std::function<void()> onClick = []() {};
		std::function<void()> onHover = []() {};
	};
}

