#pragma once
#include "Singleton.h"

/// <summary>
/// EventSystem은 Renderable 오브젝트에 부착된 EventHandler의 Event를 호출한다.
/// </summary>

namespace HDEngine
{
	class EventSystem : public Singleton<EventSystem>
	{
		friend Singleton;

	public:
		EventSystem();
		void InvokeEvent();

	private:
		// x, y is mouse input coordinate.
		// InvokeClickEvent() is called when user clicked the mouse.
		void InvokeClickEvent(float x, float y);
		void InvokeHoverEvent(float x, float y);
	};
}

