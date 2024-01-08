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

		// 디버그 관련 함수
	public:
		// 콜라이더의 디버그 데이터를 추가해두는 함수
		void AddDebugData(int flag, HDData::GameObject* obj ,HDMath::HDFLOAT4 color);
		// 콜라이더가 있는 오브젝트에 디버그 형태를 그려주는 함수.
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

