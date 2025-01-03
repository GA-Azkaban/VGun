﻿#pragma once
#include <DirectXMath.h>
#include "dllExporter.h"

#include "..\\HODO3DGraphicsInterface\\ICamera.h"

namespace RocketCore::Graphics
{
/// <summary>
/// 카메라 클래스
/// </summary>
	class Camera : public HDEngine::ICamera
	{
	public:
		Camera();
		~Camera();

		virtual void SetPositionAndRotation(const Vector3& pos, const Quaternion& rot) override;

	public:
		virtual void SetWorldTM(const Matrix& matrix) override;
		virtual void SetNearZ(float nearZ) override;
		virtual void SetFarZ(float farZ) override;
		virtual void SetAspect(float aspect) override;
		virtual void SetFOVY(float fov) override;
		virtual void SetNearHeight(float height) override;
		virtual void SetFarHeight(float height) override;
		virtual void SetAsMainCamera() override;

	public:
		DirectX::XMFLOAT3 GetPosition() const;
		void SetPosition(float x, float y, float z);
		void SetRotation(float w, float x, float y, float z);
		void SetViewMatrix(const DirectX::XMMATRIX& tm);

		void UpdateProjectionMatrix();
		void UpdateViewMatrix();

		void FrustumCulling();

		DirectX::XMMATRIX GetViewMatrix() const;				// 카메라의 로컬좌표'계'를 반환
		DirectX::XMMATRIX GetProjectionMatrix() const;		// 카메라의 투영행렬을 반환
		DirectX::XMMATRIX GetViewProjectionMatrix() const;	// 위의 두 행렬을 적절히 곱해서 반환(view의 역행렬을 곱해주겠지?)

		DirectX::XMFLOAT3 GetForward() const;
		DirectX::XMFLOAT3 GetUp() const;
		DirectX::XMFLOAT3 GetRight() const;

		float GetFOVY() const;
		float GetFOVZ() const;
		float GetAspect() const;
		float GetNearZ() const;
		float GetFarZ() const;

	private:
		DirectX::XMFLOAT3 _position;				// 위치 (월드)
		DirectX::XMFLOAT4 _rotation;				// 쿼터니언 (월드)

		float _nearZ;					// frustum의 가까운 평면까지의 거리
		float _farZ;					// frustum의 먼 평면까지의 거리
		float _aspect;					// 가로 / 세로 비율
		float _fovY;					// fov각도를 60분법으로 갖고있음. 수직각도
		float _nearWindowHeight;		// frustum의 가까운 평면의 높이
		float _farWindowHeight;			// frustum의 먼 평면의 높이
		float _nearWindowWidth;
		float _farWindowWidth;

		DirectX::XMMATRIX _worldMatrix;		// 카메라 worldTM
		DirectX::XMMATRIX _viewMatrix;		// 카메라의 로컬좌표'계' 또는 카메라 worldTM의 역행렬
		DirectX::XMMATRIX _projectionMatrix;	// 카메라의 투영 행렬

		DirectX::BoundingFrustum _boundingFrustum;
		DirectX::BoundingSphere _boundingSphere;

	public:
		static Camera* GetMainCamera();

	private:
		static Camera* _mainCamera;
	
	};
}
