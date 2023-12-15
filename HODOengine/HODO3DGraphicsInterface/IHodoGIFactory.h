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
		virtual IRenderable* CreateDebugCube() = 0;
	};
}
