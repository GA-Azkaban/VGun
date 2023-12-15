#pragma once
#include "Singleton.h"

namespace HDEngine
{
	class EventSystem : public Singleton<EventSystem>
	{
		friend Singleton;

	public:
		EventSystem();

		// x, y 좌표는 마우스가 클릭된 좌표를 받습니다.
		// 받은 좌표는 함수 내에서 0에서 1 사이의 값으로 정규화됩니다.
		void InvokeClickEvent(float x, float y);
		void InvokeHoverEvent(float x, float y);
	};
}

