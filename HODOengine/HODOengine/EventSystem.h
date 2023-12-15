#pragma once
#include "Singleton.h"

namespace HDEngine
{
	class EventSystem : public Singleton<EventSystem>
	{
		friend Singleton;

	public:
		EventSystem();

		// x, y ��ǥ�� ���콺�� Ŭ���� ��ǥ�� �޽��ϴ�.
		// ���� ��ǥ�� �Լ� ������ 0���� 1 ������ ������ ����ȭ�˴ϴ�.
		void InvokeClickEvent(float x, float y);
		void InvokeHoverEvent(float x, float y);
	};
}

