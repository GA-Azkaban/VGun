#include "ParticleSystemRenderer.h"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Particle.h"
#include <unordered_map>
#include "MathHeader.h"
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
		for (auto& e : _particleSystem->GetActivatedParticleList())
		{
			// 파티클은 ParticleSystmeRenderer의 GameObject의 Transform 기준으로 움직이는 듯 하다..
			// 파티클의 local transform을 갱신하고 GameObejct의 Transform 기준으로 해서
			// worldTM을 만들어 넘기면 되지 않을까?
			//-----------------------
			// 회전값 갱신
			// 회전은 X축 기준으로 돌아간다
			float radian = DirectX::XMConvertToRadians(e.first->GetAngle());
			Quaternion rot = Quaternion::CreateFromYawPitchRoll({ radian, 0.0f, 0.0f });
			e.first->SetRotation(Quaternion::Concatenate(e.first->GetRotation(), rot));
			// 위치값 갱신
			// forward 방향으로 speed * deltaTime 만큼 이동
			// 스케일값 갱신
			// 빌보드일 경우 카메라 방향을 바라보게 회전
			// worldTM 정보 넘겨주기
			//e.first->Get().SetWorldTM();
			// 메쉬, 매터리얼, ... 갱신
		}
	}

}
