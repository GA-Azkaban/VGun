#pragma once
#include "CameraData.h"

/// <summary>
/// �������� ���ȭ���� �����ϰ�, �׷��� ����� �׷����� ������ �Ѵ�.
/// �������� �׷��� ��ü�� �������� ������, �׷����� ���Ҹ��� �����Ѵ�.
/// </summary>

namespace HDEngine
{
	class IRenderable;

	class I3DRenderer
	{
	public:
		// �׷��Ƚ� ������ �ʱ�ȭ�Ѵ�.
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight) abstract;

		virtual void Render() abstract;

		virtual void Finalize() abstract;

		// virtual IRenderable* CreateRenderable(const std::string& modelPath) abstract;
	};

	/// dllexport �ϴ� �������� ��ȯ�ϴ� ���丮 �Լ�.

	// IRocketGraphics�� ��ӹ޴� �������� �����ؼ� ��ĳ�����Ͽ� ��ȯ�Ѵ�.
	extern "C" __declspec(dllexport) I3DRenderer* CreateGraphicsInstance();

	// �������� �ε����ʿ��� �������� �����Ҷ� ����ϴ� �Լ�.
	// �ε��ϴ��ʿ��� delete���� �ʰ� dll���� ��û�ϴ� ������ �����ߴ�.
	// ������ �������� �´� Release ������ ���ǵǾ� ���� ���̴�.
	extern "C" __declspec(dllexport) void ReleaseGraphicsInstance(I3DRenderer* instance);
}