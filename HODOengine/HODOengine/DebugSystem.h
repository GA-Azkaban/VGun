#pragma once
#include <unordered_map>
#include <vector>

#include "Singleton.h"
#include "../HODOmath/HODOmath.h"

namespace HDData
{
	class GameObject;
}

namespace HDEngine
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
		void DrawColliderDebug(HDData::GameObject* obj, HDMath::HDFLOAT4& color);
	
	private:
		struct DebugData
		{
			int index;
			HDMath::HDFLOAT4 color;
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


