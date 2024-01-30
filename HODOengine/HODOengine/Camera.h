#include "dllExporter.h"
#include "Component.h"
#include "MathHeader.h"

/// <summary>
/// 카메라는 화면에 그려질 그래픽 요소들을 출력하는 데 쓰이는 객체입니다.
/// 2023.11.09 김민정
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
		float GetAspect() const;			// 카메라 비율 screen.width/screen.height
		float GetFovX() const;				// FovX값을 60분법으로 반환
		float GetRadianFovX() const;		// FovX값을 호도법으로 반환
		float GetFovY() const;				// FovY값을 60분법으로 반환
		float GetRadianFovY() const;		// FovY값을 호도법으로 반환

		float GetNearWindowWidth() const;
		float GetNearWindowHeight() const;
		float GetFarWindowWidth() const;
		float GetFarWindowHeight() const;

		Matrix GetViewMatrix() const;	// 카메라의 로컬좌표'계'를 반환
		Matrix GetProjMatrix() const;	// 카메라의 투영행렬을 반환

		void SetNearZ(float nearZ);
		void SetFarZ(float farZ);
		void SetAspect(float aspect);
		void SetFovY(float fovY);

	public:
		void UpdateRenderData();
		//virtual void Update() override;

	private:
		void SetAsMainCamera();	// main 카메라로 바꾸는 함수. Scene에서만 호출했으면 좋겠음.

	// behavior functions
	public:
		void ShakeCamera(float deltaTime);
		void EnableCameraShake();

	private:
		float _nearZ;				// frustum의 가까운 평면까지의 거리
		float _farZ;				// frustum의 먼 평면까지의 거리
		float _aspect;				// 가로 / 세로 비율
		float _fovY;				// fov각도를 60분법으로 갖고있음
		float _nearWindowHeight;	// frustum의 가까운 평면의 높이
		float _farWindowHeight;		// frustum의 먼 평면의 높이

		Matrix _viewMatrix;		// 카메라의 로컬좌표'계'
		Matrix _projMatrix;		// 카메라의 투영 행렬

	private:
		HDEngine::ICamera* _graphicsCamera;

	private:
		bool _isShakingCamera;
		float _shootDuration;
		float _distYOnShoot;
	};
}
