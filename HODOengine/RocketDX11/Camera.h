#pragma once

#include <DirectXMath.h>
#include <cmath>


namespace RocketCore::Graphics
{
/// <summary>
/// 카메라 클래스
/// </summary>
	class Camera
	{
	public:
		Camera();
		~Camera();

	public:
		DirectX::XMFLOAT3 GetPosition() const;
		void SetPosition(float x, float y, float z);
		void SetRotation(float w, float x, float y, float z);

		float GetNearZ() const;
		float GetFarZ() const;
		float GetAspect() const;		// 카메라 비율 screen.width/screen.height
		float GetFovX() const;			// FovX값을 60분법으로 반환
		float GetRadianFovX() const;	// FovX값을 호도법으로 반환
		float GetFovY() const;			// FovY값을 60분법으로 반환
		float GetRadianFovY() const;	// FovY값을 호도법으로 반환

		float GetNearWindowWidth() const;
		float GetNearWindowHeight() const;
		float GetFarWindowWidth() const;
		float GetFarWindowHeight() const;

		void SetFrustum(float fovY, float aspect, float nearZ, float farZ);

		void UpdateViewMatrix();

		DirectX::XMMATRIX GetViewMatrix() const;				// 카메라의 로컬좌표'계'를 반환
		DirectX::XMMATRIX GetProjectionMatrix() const;		// 카메라의 투영행렬을 반환
		DirectX::XMMATRIX GetViewProjectionMatrix() const;	// 위의 두 행렬을 적절히 곱해서 반환(view의 역행렬을 곱해주겠지?)

		DirectX::XMVECTOR GetForward() const;
		DirectX::XMVECTOR GetUp() const;
		DirectX::XMVECTOR GetRight() const;

	private:
		DirectX::XMFLOAT3 _position;				// 위치 (월드)
		DirectX::XMFLOAT4 _rotation;				// 쿼터니언 (월드)

		float _nearZ;					// frustum의 가까운 평면까지의 거리
		float _farZ;					// frustum의 먼 평면까지의 거리
		float _aspect;					// 가로 / 세로 비율
		float _fovY;					// fov각도를 60분법으로 갖고있음
		float _nearWindowHeight;		// frustum의 가까운 평면의 높이
		float _farWindowHeight;			// frustum의 먼 평면의 높이

		DirectX::XMFLOAT4X4 _viewMatrix;			// 카메라의 로컬좌표'계'
		DirectX::XMFLOAT4X4 _projectionMatrix;	// 카메라의 투영 행렬
	};
}
