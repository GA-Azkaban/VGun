#pragma once

/// <summary>
/// 팩토리 객체는 카메라, 메쉬, 라이트와 같은 렌더링에 필요한 객체들을 생성한다.
/// 2023.11.10 김민정
/// </summary>

namespace HDEngine
{
	class ICamera;
	class IRenderable;

	class IFactory
	{
	public:
		virtual ICamera* CreateCamera() = 0;
	};

	/// dllexport 하는 렌더러를 반환하는 팩토리 함수.

// IRocketGraphics를 상속받는 렌더러를 생성해서 업캐스팅하여 반환한다.
	extern "C" __declspec(dllexport) IFactory* CreateGraphicsObjectFactory();

	// 렌더러를 로드한쪽에서 렌더러를 해제할때 사용하는 함수.
	// 로드하는쪽에서 delete하지 않고 dll한테 요청하는 식으로 구성했다.
	// 각각의 렌더러에 맞는 Release 동작이 정의되어 있을 것이다.
	extern "C" __declspec(dllexport) void ReleaseFactory(IFactory* instance);
}
