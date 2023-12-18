#include "dllExporter.h"
#include "Component.h"
#include "..\\HODOmath\\HODOmath.h"

/// <summary>
/// ī�޶�� ȭ�鿡 �׷��� �׷��� ��ҵ��� ����ϴ� �� ���̴� ��ü�Դϴ�.
/// 2023.11.09 �����
/// </summary>

namespace HDEngine
{
	class ICamera;
}

namespace HDData
{
	class Scene;

	class HODO_API Camera : public Component
	{
		friend Scene;
	public:
		Camera();

	public:
		float GetNearZ() const;
		float GetFarZ() const;
		float GetAspect() const;			// ī�޶� ���� screen.width/screen.height
		float GetFovX() const;				// FovX���� 60�й����� ��ȯ
		float GetRadianFovX() const;		// FovX���� ȣ�������� ��ȯ
		float GetFovY() const;				// FovY���� 60�й����� ��ȯ
		float GetRadianFovY() const;		// FovY���� ȣ�������� ��ȯ

		float GetNearWindowWidth() const;
		float GetNearWindowHeight() const;
		float GetFarWindowWidth() const;
		float GetFarWindowHeight() const;

		HDMath::HDFLOAT4X4 GetViewMatrix() const;	// ī�޶��� ������ǥ'��'�� ��ȯ
		HDMath::HDFLOAT4X4 GetProjMatrix() const;	// ī�޶��� ��������� ��ȯ

		void SetNearZ(float nearZ);
		void SetFarZ(float farZ);
		void SetAspect(float aspect);
		void SetFovY(float fovY);

	public:
		void UpdateRenderData();

	private:
		void SetAsMainCamera();	// main ī�޶�� �ٲٴ� �Լ�. Scene������ ȣ�������� ������.

	private:
		float _nearZ;				// frustum�� ����� �������� �Ÿ�
		float _farZ;				// frustum�� �� �������� �Ÿ�
		float _aspect;				// ���� / ���� ����
		float _fovY;				// fov������ 60�й����� ��������
		float _nearWindowHeight;	// frustum�� ����� ����� ����
		float _farWindowHeight;		// frustum�� �� ����� ����

		HDMath::HDFLOAT4X4 _viewMatrix;		// ī�޶��� ������ǥ'��'
		HDMath::HDFLOAT4X4 _projMatrix;		// ī�޶��� ���� ���

	private:
		HDEngine::ICamera* _graphicsCamera;
	};
}
