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

namespace HDEngine
{
	class InputSystem : public Singleton<InputSystem>
	{
		friend Singleton;
	private:
		InputSystem() = default;

	public:
		// �ʱ�ȭ
		void Initialize(HWND hWnd, int screenWidth, int screenHeight);
		// ��ǲ �ý��� ������Ʈ
		void Update();
		// ���� ����Ŭ ������ ������ �͵�
		void Flush();

	public:
		bool GetKeyDown(int keyCode);
		bool GetKeyUp(int keyCode);
		bool GetKeyPressing(int keyCode);

	private:
		bool _currentKeyState[256];
		bool _previousKeyState[256];
	
	public:
		HDMath::HDFLOAT2 GetMousePosition();
		HDMath::HDFLOAT2 GetMousePositionNormalized();
	
	private:
		HWND _hWnd;
		POINT _mousePoint;

		int _screenWidth;
		int _screenHeight;
		int _widthOffset;
		int _heightOffset;

		HDMath::HDFLOAT2 _currentMousePosition;
		HDMath::HDFLOAT2 _previousMousePosition;
	};
}


