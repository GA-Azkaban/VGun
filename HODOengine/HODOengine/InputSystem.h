#pragma once
#include <windows.h>

#include "../HODOmath/HODOmath.h"
#include "Singleton.h"
#include <unordered_map>
#include <unordered_set>
/// <summary>
/// 오수안
/// 키 인풋을 위한 기초 시스템
/// </summary>

namespace HDEngine
{
	class InputSystem : public Singleton<InputSystem>
	{
		friend Singleton;
	private:
		InputSystem() = default;

	public:
		// 초기화
		void Initialize(HWND hWnd, int screenWidth, int screenHeight);
		// 인풋 시스템 업데이트
		void Update();
		// 다음 싸이클 직전에 정리할 것들
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
		HDMath::HDFLOAT2 GetMouseDelta();
	
	private:
		HWND _hWnd;
		POINT _mousePoint;

		int _screenWidth;
		int _screenHeight;
		int _widthOffset;
		int _heightOffset;

		HDMath::HDFLOAT2 _currentMousePosition;
		HDMath::HDFLOAT2 _previousMousePosition;
		HDMath::HDFLOAT2 _currentMouseDelta;
		HDMath::HDFLOAT2 _previousMouseDelta;
	};
}


