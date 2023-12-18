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
	};

	/// dllexport �ϴ� �������� ��ȯ�ϴ� ���丮 �Լ�.

// IRocketGraphics�� ��ӹ޴� �������� �����ؼ� ��ĳ�����Ͽ� ��ȯ�Ѵ�.
	extern "C" __declspec(dllexport) IFactory* CreateGraphicsObjectFactory();

	// �������� �ε����ʿ��� �������� �����Ҷ� ����ϴ� �Լ�.
	// �ε��ϴ��ʿ��� delete���� �ʰ� dll���� ��û�ϴ� ������ �����ߴ�.
	// ������ �������� �´� Release ������ ���ǵǾ� ���� ���̴�.
	extern "C" __declspec(dllexport) void ReleaseFactory(IFactory* instance);
}
