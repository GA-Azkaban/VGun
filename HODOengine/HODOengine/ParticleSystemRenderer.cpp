#include "ParticleSystemRenderer.h"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "TimeSystem.h"
#include "MathHeader.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "Camera.h"
#include "../HODO3DGraphicsInterface/IParticle.h"
#include <unordered_map>
using namespace DirectX;

namespace HDData
{

	ParticleSystemRenderer::ParticleSystemRenderer()
	{

	}

	ParticleSystemRenderer::~ParticleSystemRenderer()
	{
		_particleSystem = nullptr;
	}

	void ParticleSystemRenderer::Start()
	{
		_particleSystem = GetGameObject()->GetComponent<HDData::ParticleSystem>();
	}

	void ParticleSystemRenderer::Update()
	{
		float deltaTime = HDEngine::TimeSystem::Instance().GetDeltaTime();
		Matrix worldTM = GetGameObject()->GetTransform()->GetWorldTM();
		for (auto& e : _particleSystem->GetActivatedParticleList())
		{
			// 회전값 갱신
			// 회전은 Z축 기준으로 돌아간다
			float radian = DirectX::XMConvertToRadians(e.first->GetAngle());
			Quaternion rot = Quaternion::CreateFromYawPitchRoll({ 0.0f, 0.0f, radian });
			e.first->SetRotation(Quaternion::Concatenate(e.first->GetRotation(), rot));
			//e.first->SetRotation(rot);
			// 위치값 갱신
			// forward 방향으로 speed * deltaTime 만큼 이동
			Matrix rotationMat = Matrix::CreateFromQuaternion(e.first->GetRotation());
			Vector3 forward = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), rotationMat);
			Vector3 right = Vector3::Transform(Vector3(1.0f, 0.0f, 0.0f), rotationMat);
			Vector3 up = Vector3::Transform(Vector3(0.0f, 1.0f, 0.0f), rotationMat);
			Vector3 delta{ 0.0f, 0.0f, 0.0f };
			float speed = e.first->GetSpeed();
			delta.x = (right.x + up.x) * speed * deltaTime;
			delta.y = (right.y + up.y) * speed * deltaTime;
			delta.z = (forward.z) * speed * deltaTime;
			Vector3 currentPos = e.first->GetPosition();
			e.first->SetPosition(delta + currentPos);
			// 빌보드일 경우 카메라 방향을 바라보게 회전
			if (_particleSystem->rendererModule.renderMode == HDEngine::ParticleSystemRenderMode::Billboard)
			{
				HDData::Camera* mainCam = HDEngine::SceneSystem::Instance().GetCurrentScene()->GetMainCamera();
				Vector3 cameraPosition = mainCam->GetTransform()->GetPosition();
				float radian = std::atan2(0.0 - cameraPosition.x, 0.0 - cameraPosition.z);
				Quaternion rotToCam = Quaternion::CreateFromYawPitchRoll({ 0, radian, 0 });
				// worldTM 정보 넘겨주기
				// 파티클의 localTM을 구해서 파티클렌더러의 worldTM과 곱해서 넘겨준다.
				Matrix localTM;
				localTM *= Matrix::CreateScale(e.first->GetScale());
				localTM *= Matrix::CreateFromQuaternion(Quaternion::Concatenate(e.first->GetRotation(), rotToCam));
				localTM *= Matrix::CreateTranslation(e.first->GetPosition());
				e.first->SetWorldTM(DirectX::XMMatrixMultiply(localTM, worldTM));
			}
			else
			{
				// worldTM 정보 넘겨주기
				// 파티클의 localTM을 구해서 파티클렌더러의 worldTM과 곱해서 넘겨준다.
				Matrix localTM;
				localTM *= Matrix::CreateScale(e.first->GetScale());
				localTM *= Matrix::CreateFromQuaternion(e.first->GetRotation());
				localTM *= Matrix::CreateTranslation(e.first->GetPosition());
				e.first->SetWorldTM(DirectX::XMMatrixMultiply(localTM, worldTM));
			}
			// 스케일값 갱신
			float scale = e.first->GetSize();
			e.first->SetScale({ scale, scale, scale });
		}
	}

}
