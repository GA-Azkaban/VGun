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

	/// <summary>
	///  ����� ���� �Լ�
	/// </summary>
	public:
		// �ݶ��̴��� ����� �����͸� �߰��صδ� �Լ�
		void AddDebugData(int flag, HDData::GameObject* obj ,HDMath::HDFLOAT4 color);
		// �ݶ��̴��� �ִ� ������Ʈ�� ����� ���¸� �׷��ִ� �Լ�.
		void DrawColliderDebug(HDData::GameObject* obj, HDMath::HDFLOAT4 color);
	
		struct debugData
		{
			int flag = 0;
			HDData::GameObject* obj = nullptr;
			HDMath::HDFLOAT4 color = HDMath::HDFLOAT4{1.f, 0.f, 0.f, 0.f};
		};

	private:
		std::vector<debugData*> _colliderDebugData;

	public:
		void SetDebugOn(int index);
		void SetDebugOff();
		

	private:
		int _index;
		bool _isDebugMode = false;
	};
}


