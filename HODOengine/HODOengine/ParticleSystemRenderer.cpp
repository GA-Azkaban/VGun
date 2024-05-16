#include "ParticleSystemRenderer.h"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Particle.h"
#include "TimeSystem.h"
#include "MathHeader.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "Camera.h"
#include <unordered_map>
using namespace DirectX;

namespace HDData
{

	ParticleSystemRenderer::ParticleSystemRenderer()
	{
		_particleSystem = GetGameObject()->GetComponent<HDData::ParticleSystem>();
	}

	ParticleSystemRenderer::~ParticleSystemRenderer()
	{
		_particleSystem = nullptr;
	}

	void ParticleSystemRenderer::Update()
	{
		float deltaTime = HDEngine::TimeSystem::Instance().GetDeltaTime();
		Matrix worldTM = GetGameObject()->GetTransform()->GetWorldTM();
		for (auto& e : _particleSystem->GetActivatedParticleList())
		{
			// 회전값 갱신
			// 회전은 X축 기준으로 돌아간다
			float radian = DirectX::XMConvertToRadians(e.first->GetAngle());
			Quaternion rot = Quaternion::CreateFromYawPitchRoll({ radian, 0.0f, 0.0f });
			e.first->SetRotation(Quaternion::Concatenate(e.first->GetRotation(), rot));
			// 위치값 갱신
			// forward 방향으로 speed * deltaTime 만큼 이동
			Matrix rotationMat = Matrix::CreateFromQuaternion(e.first->GetRotation());
			Vector3 forward = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), rotationMat);
			Vector3 delta{ 0.0f, 0.0f, 0.0f };
			delta.z = forward.z * e.first->GetSpeed() * deltaTime;
			Vector3 currentPos = e.first->GetPosition();
			e.first->SetPosition(delta + currentPos);
			// 빌보드일 경우 카메라 방향을 바라보게 회전
			if (e.first->GetRenderMode() == HDEngine::ParticleSystemRenderMode::Billboard)
			{		
				HDData::Camera* mainCam = HDEngine::SceneSystem::Instance().GetCurrentScene()->GetMainCamera();
				Vector3 cameraPosition = mainCam->GetTransform()->GetPosition();
				float radian = std::atan2(0.0 - cameraPosition.x, 0.0 - cameraPosition.z);
				Quaternion rotToCam = Quaternion::CreateFromYawPitchRoll({ 0, radian, 0 });
				e.first->SetRotation(Quaternion::Concatenate(e.first->GetRotation(), rotToCam));
			}
			// 스케일값 갱신
			float scale = e.first->GetSize();
			e.first->SetScale(scale, scale, scale);
			// worldTM 정보 넘겨주기
			// 파티클의 localTM을 구해서 파티클렌더러의 worldTM과 곱해서 넘겨준다.
			Matrix localTM;
			localTM *= Matrix::CreateScale(e.first->GetScale());
			localTM *= Matrix::CreateFromQuaternion(e.first->GetRotation());
			localTM *= Matrix::CreateTranslation(e.first->GetPosition());
			e.first->Get().SetWorldTM(DirectX::XMMatrixMultiply(localTM, worldTM));
			// 컬러값 갱신
			e.first->Get().SetColor(e.first->GetColor());
			// 메쉬, 매터리얼 갱신은 어떻게 할까?
		}
	}

}
