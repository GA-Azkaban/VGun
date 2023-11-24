#pragma once
#include <Windows.h>

/// <summary>
/// 렌더러는 출력화면을 제어하고, 그려낼 대상을 그려내는 역할을 한다.
/// 렌더러는 그려낼 객체를 생성하지 않으며, 그려내는 역할만을 수행한다.
/// </summary>

namespace hodoGI
{
	class I3DRenderer
	{
	public:
		virtual ~I3DRenderer() {};

		// 초기화는 생성자에서 한다.
		//virtual void Initialize() = 0;

		// 출력할 대상 윈도우를 설정한다.
		virtual void SetOutputWindow(UINT64 hWnd) = 0;

		// 시간에 따라 상태를 변화한다.
		virtual void Update(float deltaTime) = 0;

		// 화면에 그린다.
		virtual void Render() = 0;

		// 해상도 변경 시 호출된다.
		virtual void ResizeResolution(UINT width, UINT height) = 0;
	};
}