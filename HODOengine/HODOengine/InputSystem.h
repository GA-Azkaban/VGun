#pragma once
#include <windows.h>

#include "../HODOmath/HODOmath.h"
#include "Singleton.h"
#include <unordered_map>
#include <unordered_set>
/// <summary>
/// ������
/// Ű ��ǲ�� ���� ���� �ý���
/// </summary>

namespace hodoEngine
{

	class InputSystem : public Singleton<InputSystem>
	{
		friend Singleton;
	private:
		InputSystem() = default;

	public:
		void Initialize();
		void Update();

	public:
		bool GetKeyDown(int keyCode);
		bool GetKeyUp(int keyCode);
		bool GetKeyPressing(int keyCode);

	public:
		HDMaths::HDFLOAT2 GetMousePosition();
		HDMaths::HDFLOAT2 GetMousePositionNormalized();
		
	private:
		bool _currentKeyState[256];
		bool _previousKeyState[256];
	};
}


