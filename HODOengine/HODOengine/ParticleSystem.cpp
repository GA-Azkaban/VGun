#include "ParticleSystem.h"
#include "GameObject.h"
#include "ParticleSystemRenderer.h"
#include "TimeSystem.h"
#include "GraphicsObjFactory.h"
#include "RandomGenerator.h"
#include "../HODO3DGraphicsInterface/IParticle.h"
#include "../HODO3DGraphicsInterface/IParticleSystem.h"

namespace HDData
{	
	ParticleSystem::ParticleSystem()
		: main(), colorOverLifetime(), emission(),
		sizeOverLifetime(), rotationOverLifetime(),
		useAutoRandomSeed(true), time(0.0f),
		_isPlaying(false), _accumulatedDeltaTime(0.0f),
		_particleSystem(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateParticleSystem())
	{
		_gen = HDEngine::RandomGenerator::Instance().MersenneTwiste();
	}

	ParticleSystem::~ParticleSystem()
	{

	}

	void ParticleSystem::Play()
	{
		_isPlaying = true;
		_accumulatedDeltaTime = 0.0f;
		emission.GetBurst().currentCycleCount = 0;
		// 자식 오브젝트에 Particle System이 있다면 같이 실행해야 한다.
		for (auto e : GetGameObject()->GetChildGameObjects())
		{
			e->GetComponent<ParticleSystem>()->Play();
		}
	}

	void ParticleSystem::Stop()
	{
		_isPlaying = false;
		Clear();
		// 자식 오브젝트에 Particle System이 있다면 같이 멈춰야 한다.
		for (auto e : GetGameObject()->GetChildGameObjects())
		{
			e->GetComponent<ParticleSystem>()->Stop();
		}
	}

	void ParticleSystem::Clear()
	{
		for (auto& e : _activatedParticles)
		{
			_particleSystem->DestroyParticle(e.first);
		}
		_activatedParticles.clear();
	}

	bool ParticleSystem::IsPlaying() const
	{
		return _isPlaying;
	}

	std::unordered_map<HDEngine::IParticle*, std::pair<float, float>>& ParticleSystem::GetActivatedParticleList()
	{
		return _activatedParticles;
	}

	void ParticleSystem::OnEnable()
	{
		_particleSystem->SetActive(true);
	}

	void ParticleSystem::OnDisable()
	{
		_particleSystem->SetActive(false);
	}

	void ParticleSystem::Start()
	{
		GetGameObject()->AddComponent<ParticleSystemRenderer>();
		_particleSystem->SetMesh(rendererModule.mesh);
		_particleSystem->SetMaterial(rendererModule.material->Get());
		_particleSystem->SetRenderMode(rendererModule.renderMode);
		_particleSystem->SetAlphaBlend(rendererModule.alphaBlending);
	}

	void ParticleSystem::Update()
	{
		if (!_isPlaying)
		{
			return;
		}

		float deltaTime = HDEngine::TimeSystem::Instance().GetDeltaTime();

		// emission의 지정된 시간에 지정된 개수만큼 파티클 생성
		// 파티클은 풀에서 꺼내온다
		Burst& burst = emission.GetBurst();
		if (_accumulatedDeltaTime >= burst.time)
		{
			if (burst.currentCycleCount < burst.cycleCount)
			{
				if (burst.count > -1)
				{
					for (int i = 0; i < burst.count; ++i)
					{
						SummonParticleFromPool();
					}
				}
				else
				{
					std::uniform_int_distribution<> int_dist(burst.minCount, burst.maxCount);
					int randomCount = int_dist(_gen);
					for (int i = 0; i < randomCount; ++i)
					{
						SummonParticleFromPool();
					}
				}
				++(burst.currentCycleCount);
			}
		}

		// 활성화 되어있는 파티클들 정보 업데이트
		float dt = _accumulatedDeltaTime / main.duration;
		for (auto& p : _activatedParticles)
		{
			if (colorOverLifetime.enabled)
			{
				p.first->SetColor(colorOverLifetime.color.Evaluate(dt));
			}
			if (sizeOverLifetime.enabled)
			{
				p.first->SetSize(p.first->GetStartSize() * sizeOverLifetime.size.Evaluate(dt));
			}
			if (rotationOverLifetime.enabled)
			{
				p.first->SetAngle(rotationOverLifetime.angularVelocity.Evaluate(dt) * deltaTime);
			}

			// 파티클의 라이프타임과 파티클의 누적 델타타임을 비교해서 시간이 다 된 파티클을 지워준다
			if (p.second.second >= p.second.first)
			{
				_lifeOverParticles.push_back(p.first);
			}
			p.second.second += deltaTime;
		}

		// 시간 다 된 파티클 삭제
		for (int i = 0; i < _lifeOverParticles.size(); ++i)
		{
			auto iter = _activatedParticles.find(_lifeOverParticles[i]);
			if (iter != _activatedParticles.end())
			{
				_particleSystem->DestroyParticle(iter->first);
				_activatedParticles.erase(iter);
			}
		}
		_lifeOverParticles.clear();

		if (_accumulatedDeltaTime > main.duration)
		{
			if (main.loop)
			{
				_accumulatedDeltaTime = 0.0f;
				burst.currentCycleCount = 0;
				for (auto iter = _activatedParticles.begin(); iter != _activatedParticles.end(); ++iter)
				{
					iter->first->SetGravityVelocity(main.initialVelocity);
				}
			}
			else
			{
				_isPlaying = false;
				// 활성화 되어 있던 파티클 삭제
				for (auto iter = _activatedParticles.begin(); iter != _activatedParticles.end(); ++iter)
				{
					_particleSystem->DestroyParticle(iter->first);
				}
				_activatedParticles.clear();
			}
		}
		else
		{
			_accumulatedDeltaTime += deltaTime;
		}
	}

	void ParticleSystem::SummonParticleFromPool()
	{
		std::uniform_real_distribution<float> real_dist(main.minStartLifetime, main.maxStartLifetime);
		// 파티쿨 풀에서 가져오기
		HDEngine::IParticle* particle = _particleSystem->SummonParticle();

		if (particle != nullptr)
		{
			// 파티클 정보 세팅
			std::uniform_int_distribution<> color_dist_r(main.minStartColor.x, main.maxStartColor.x);
			std::uniform_int_distribution<> color_dist_g(main.minStartColor.y, main.maxStartColor.y);
			std::uniform_int_distribution<> color_dist_b(main.minStartColor.z, main.maxStartColor.z);
			std::uniform_int_distribution<> color_dist_a(main.minStartColor.w, main.maxStartColor.w);
			int r = color_dist_r(_gen);
			int g = color_dist_g(_gen);
			int b = color_dist_b(_gen);
			int a = color_dist_a(_gen);
			particle->SetColor(DirectX::XMINT4{ r, g, b, a });
			std::uniform_real_distribution<float> speed_dist(main.minStartSpeed, main.maxStartSpeed);
			float speed = speed_dist(_gen);
			particle->SetSpeed(speed);
			std::uniform_real_distribution<float> size_dist(main.minStartSize, main.maxStartSize);
			float size = size_dist(_gen);
			particle->SetStartSize(size);
			particle->SetSize(size);
			particle->SetScale(size, size, size);
			std::uniform_real_distribution<float> rotation_dist(main.minStartRotation, main.maxStartRotation);
			float angle = rotation_dist(_gen);
			particle->SetAngle(0.0f);
			particle->SetPosition(0.0f, 0.0f, 0.0f);
			if (rendererModule.renderMode == HDEngine::ParticleSystemRenderMode::Mesh)
			{
				float radian = DirectX::XMConvertToRadians(angle);
				Quaternion rot = Quaternion::CreateFromYawPitchRoll({ 0.0f, radian, 0.0f });
				particle->SetRotation(rot);
			}
			else
			{
				float radian = DirectX::XMConvertToRadians(angle);
				Quaternion rot = Quaternion::CreateFromYawPitchRoll({ 0.0f, 0.0f, radian });
				particle->SetRotation(rot);
			}
			// max size일 때 gravityModifier의 1배
			// 사이즈 작아질 수록 gravityModifier도 작아진다
			float gravityModifier = main.gravityModifier;
			if (limitVelocityOverLifetime.drag)
			{
				gravityModifier = main.gravityModifier * (size / main.maxStartSize);
			}
			particle->SetGravityModifier(gravityModifier);
			particle->SetGravityVelocity(main.initialVelocity);
			float randomLifetime = real_dist(_gen);
			_activatedParticles.insert(std::make_pair(particle, std::make_pair(randomLifetime, 0.0f)));
		}
	}

}
