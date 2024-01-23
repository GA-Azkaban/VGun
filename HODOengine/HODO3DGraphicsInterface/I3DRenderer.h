#pragma once

/// <summary>
/// 렌더러는 출력화면을 제어하고, 그려낼 대상을 그려내는 역할을 한다.
/// 렌더러는 그려낼 객체를 생성하지 않으며, 그려내는 역할만을 수행한다.
/// </summary>

namespace HDEngine
{
	class I3DRenderer
	{
	public:
		// 그래픽스 엔진을 초기화한다.
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight) abstract;

		virtual void Update(float deltaTime, bool isDebug = false) abstract;

		virtual void Render() abstract;

		virtual void Finalize() abstract;
	};

	/// dllexport 하는 렌더러를 반환하는 팩토리 함수.

	// IRocketGraphics를 상속받는 렌더러를 생성해서 업캐스팅하여 반환한다.
	extern "C" __declspec(dllexport) I3DRenderer* CreateGraphicsInstance();

	// 렌더러를 로드한쪽에서 렌더러를 해제할때 사용하는 함수.
	// 로드하는쪽에서 delete하지 않고 dll한테 요청하는 식으로 구성했다.
	// 각각의 렌더러에 맞는 Release 동작이 정의되어 있을 것이다.
	extern "C" __declspec(dllexport) void ReleaseGraphicsInstance(I3DRenderer* instance);
}
