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

	/// <summary>
	///  ����� ���� �Լ�
	/// </summary>
	public:
		// �ݶ��̴��� ����� �����͸� �߰��صδ� �Լ�
		void AddDebugData(int flag, hodoData::GameObject* obj ,HDMaths::HDFLOAT4 color);
		// �ݶ��̴��� �ִ� ������Ʈ�� ����� ���¸� �׷��ִ� �Լ�.
		void DrawColliderDebug(hodoData::GameObject* obj, HDMaths::HDFLOAT4 color);
	
		struct debugData
		{
			int flag = 0;
			hodoData::GameObject* obj = nullptr;
			HDMaths::HDFLOAT4 color = HDMaths::HDFLOAT4{1.f, 0.f, 0.f, 0.f};
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


