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

		// 콜라이더가 있는 오브젝트에 디버그 형태를 그려주는 함수.
		// 콜라이더 생성 시 자동으로 호출된다.
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


