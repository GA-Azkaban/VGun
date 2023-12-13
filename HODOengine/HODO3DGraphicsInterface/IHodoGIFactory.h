#pragma once

/// <summary>
/// ���丮 ��ü�� ī�޶�, �޽�, ����Ʈ�� ���� �������� �ʿ��� ��ü���� �����Ѵ�.
/// 2023.11.10 �����
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