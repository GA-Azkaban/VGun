#pragma once
#include <DirectXMath.h>
#include "dllExporter.h"

#include "..\\HODO3DGraphicsInterface\\ICamera.h"

namespace RocketCore::Graphics
{
/// <summary>
/// ī�޶� Ŭ����
/// </summary>
	class Camera : public HDEngine::ICamera
	{
	public:
		Camera();
		~Camera();


		virtual void SetPositionAndRotation(const HDMath::HDFLOAT3& pos, const HDMath::HDQuaternion& rot) override;

	public:
		virtual void SetWorldTM(const HDMath::HDFLOAT4X4& matrix) override;
		virtual void SetNearZ(float near) override;
		virtual void SetFarZ(float far) override;
		virtual void SetAspect(float aspect) override;
		virtual void SetFOVY(float fov) override;
		virtual void SetNearHeight(float height) override;
		virtual void SetFarHeight(float height) override;
		virtual void SetAsMainCamera() override;

	public:
		DirectX::XMFLOAT3 GetPosition() const;
		void SetPosition(float x, float y, float z);
		void SetRotation(float w, float x, float y, float z);

		void UpdateProjectionMatrix();
		void UpdateViewMatrix();

		DirectX::XMMATRIX GetViewMatrix() const;				// ī�޶��� ������ǥ'��'�� ��ȯ
		DirectX::XMMATRIX GetProjectionMatrix() const;		// ī�޶��� ��������� ��ȯ
		DirectX::XMMATRIX GetViewProjectionMatrix() const;	// ���� �� ����� ������ ���ؼ� ��ȯ(view�� ������� �����ְ���?)

		DirectX::XMVECTOR GetForward() const;
		DirectX::XMVECTOR GetUp() const;
		DirectX::XMVECTOR GetRight() const;

	private:
		DirectX::XMFLOAT3 _position;				// ��ġ (����)
		DirectX::XMFLOAT4 _rotation;				// ���ʹϾ� (����)

		float _nearZ;					// frustum�� ����� �������� �Ÿ�
		float _farZ;					// frustum�� �� �������� �Ÿ�
		float _aspect;					// ���� / ���� ����
		float _fovY;					// fov������ 60�й����� ��������
		float _nearWindowHeight;		// frustum�� ����� ����� ����
		float _farWindowHeight;			// frustum�� �� ����� ����

		DirectX::XMFLOAT4X4 _worldMatrix;		// ī�޶� worldTM
		DirectX::XMFLOAT4X4 _viewMatrix;		// ī�޶��� ������ǥ'��' �Ǵ� ī�޶� worldTM�� �����
		DirectX::XMFLOAT4X4 _projectionMatrix;	// ī�޶��� ���� ���

	public:
		static Camera* GetMainCamera();

	private:
		static Camera* _mainCamera;
	};
}
