#pragma once

/// <summary>
/// ���丮 ��ü�� ī�޶�, �޽�, ����Ʈ�� ���� �������� �ʿ��� ��ü���� �����Ѵ�.
/// 2023.11.10 �����
/// </summary>

namespace hodoGI
{
	class ICamera;

	class IFactory
	{
	public:
		virtual ICamera* CreateCamera() = 0;
		virtual ICamera* CreateStaticMesh() = 0;
		virtual ICamera* CreateSkinnedMesh() = 0;
		virtual ICamera* CreateDirectionalLight() = 0;
	};
}