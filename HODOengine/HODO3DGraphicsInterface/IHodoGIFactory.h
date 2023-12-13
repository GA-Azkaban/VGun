#pragma once

/// <summary>
/// 팩토리 객체는 카메라, 메쉬, 라이트와 같은 렌더링에 필요한 객체들을 생성한다.
/// 2023.11.10 김민정
/// </summary>

namespace HDEngine
{
	class ICamera;
	class IDebugCube;

	class IFactory
	{
	public:
		virtual ICamera* CreateCamera() = 0;
		virtual IDebugCube* CreateDebugCube() = 0;
		//virtual IStaticMesh* CreateStaticMesh() = 0;
		//virtual ISkinnedMesh* CreateSkinnedMesh() = 0;
		//virtual IDirectionalLight* CreateDirectionalLight() = 0;
	};
}