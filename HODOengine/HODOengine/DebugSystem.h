#pragma once
#include <unordered_map>
#include <vector>

#include "Singleton.h"
#include "../HODOmath/HODOmath.h"

namespace hodoData
{
	class GameObject;
}

namespace hodoEngine
{
	class DebugSystem : public Singleton<DebugSystem>
	{
		friend Singleton;

	private:
		DebugSystem() = default;

	public:
		void Initialize();
		void Update();

		// �ݶ��̴��� �ִ� ������Ʈ�� ����� ���¸� �׷��ִ� �Լ�.
		// �ݶ��̴� ���� �� �ڵ����� ȣ��ȴ�.
		void DrawColliderDebug(hodoData::GameObject* obj, HDMaths::HDFLOAT4& color);
	
	private:
		struct DebugData
		{
			int index;
			HDMaths::HDFLOAT4 color;
		};

		std::vector<DebugData> _debugObj;

	public:
		void SetDebugOn(int index);
		void SetDebugOff();
		

	private:
		int _index;
		bool _isDebugMode = false;
	};
}


