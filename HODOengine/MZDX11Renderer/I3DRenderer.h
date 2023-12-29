#pragma once
///
/// �������� �������̽�
/// 2023.06.19 MJKIM

namespace MZRenderer
{
	class I3DRenderer
	{
	public:
		I3DRenderer() {};
		virtual ~I3DRenderer() {};

		// ���� �ʱ�ȭ
		virtual bool Initialize() abstract;
		virtual void SetOutputWindow(unsigned int hWnd) abstract;
		// ������Ʈ
		virtual void Update(float deltaTime) abstract;

		// ����
		virtual void Render() abstract;

		// ���� ����
		virtual void Finalize() abstract;

	public:
		// â ��ȯ ����
		virtual void ResizeResolution(unsigned int width, unsigned int height) abstract;

	public:
		// ���콺 �Է� ����
		virtual void OnMouseDown(int btnState, int x, int y) abstract;
		virtual void OnMouseUp(int x, int y) abstract;
		virtual void OnMouseMove(int btnState, int x, int y) abstract;
	};

}
