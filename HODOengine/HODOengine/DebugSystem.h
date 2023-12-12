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
	///  디버그 관련 함수
	/// </summary>
	public:
		// 콜라이더의 디버그 데이터를 추가해두는 함수
		void AddDebugData(int flag, hodoData::GameObject* obj ,HDMaths::HDFLOAT4 color);
		// 콜라이더가 있는 오브젝트에 디버그 형태를 그려주는 함수.
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


