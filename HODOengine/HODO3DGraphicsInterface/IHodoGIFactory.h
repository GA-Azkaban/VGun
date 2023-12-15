#pragma once

/// <summary>
/// ���丮 ��ü�� ī�޶�, �޽�, ����Ʈ�� ���� �������� �ʿ��� ��ü���� �����Ѵ�.
/// 2023.11.10 �����
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
