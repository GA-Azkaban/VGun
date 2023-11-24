#pragma once
#include <Windows.h>

/// <summary>
/// �������� ���ȭ���� �����ϰ�, �׷��� ����� �׷����� ������ �Ѵ�.
/// �������� �׷��� ��ü�� �������� ������, �׷����� ���Ҹ��� �����Ѵ�.
/// </summary>

namespace hodoGI
{
	class I3DRenderer
	{
	public:
		virtual ~I3DRenderer() {};

		// �ʱ�ȭ�� �����ڿ��� �Ѵ�.
		//virtual void Initialize() = 0;

		// ����� ��� �����츦 �����Ѵ�.
		virtual void SetOutputWindow(UINT64 hWnd) = 0;

		// �ð��� ���� ���¸� ��ȭ�Ѵ�.
		virtual void Update(float deltaTime) = 0;

		// ȭ�鿡 �׸���.
		virtual void Render() = 0;

		// �ػ� ���� �� ȣ��ȴ�.
		virtual void ResizeResolution(UINT width, UINT height) = 0;
	};
}