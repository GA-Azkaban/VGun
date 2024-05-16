#include "ParticleSystem.h"
#include "GameObject.h"
#include "ParticleSystemRenderer.h"
#include "TimeSystem.h"
#include "Particle.h"
#include "ParticlePool.h"
#include "GraphicsObjFactory.h"

namespace HDData
{

	ParticleSystem::ParticleSystem()
		: main(), colorOverLifetime(), emission(),
		sizeOverLifetime(), rotationOverLifetime(),
		useAutoRandomSeed(true), time(0.0f),
		_isPlaying(false), _accumulatedDeltaTime(0.0f),
		rd(), gen(rd())
	{
		GetGameObject()->AddComponent<ParticleSystemRenderer>();
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
	}

	void ParticleSystem::Clear()
	{
		for (auto& e : _activatedParticles)
		{
			HDEngine::ParticlePool::Instance().Retrieve(e.first);
		}
		_activatedParticles.clear();
	}

	std::unordered_map<HDData::Particle*, std::pair<float, float>>& ParticleSystem::GetActivatedParticleList()
	{
		return _activatedParticles;
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
				std::uniform_real_distribution real_dist(main.minStartLifetime, main.maxStartLifetime);
				if (burst.count > -1)
				{
					for (int i = 0; i < burst.count; ++i)
					{
						// 파티쿨 풀에서 가져오기
						HDData::Particle* particle = HDEngine::ParticlePool::Instance().SummonParticle();
						// 파티클 정보 세팅
						particle->SetMesh(rendererModule.mesh);
						particle->SetMaterial(rendererModule.material);
						particle->SetRenderMode(rendererModule.renderMode);
						std::uniform_int_distribution color_dist_r(main.minStartColor.x, main.maxStartColor.x);
						std::uniform_int_distribution color_dist_g(main.minStartColor.y, main.maxStartColor.y);
						std::uniform_int_distribution color_dist_b(main.minStartColor.z, main.maxStartColor.z);
						std::uniform_int_distribution color_dist_a(main.minStartColor.w, main.maxStartColor.w);
						int r = color_dist_r(gen);
						int g = color_dist_g(gen);
						int b = color_dist_b(gen);
						int a = color_dist_a(gen);
						particle->SetColor(DirectX::XMINT4{ r, g, b, a });
						std::uniform_real_distribution speed_dist(main.minStartSpeed, main.maxStartSpeed);
						float speed = speed_dist(gen);
						particle->SetSpeed(speed);
						std::uniform_real_distribution size_dist(main.minStartSize, main.maxStartSize);
						float size = size_dist(gen);
						particle->SetSize(size);
						std::uniform_real_distribution rotation_dist(main.minStartRotation, main.maxStartRotation);
						float angle = rotation_dist(gen);
						particle->SetAngle(angle);
						float randomLifetime = real_dist(gen);
						_activatedParticles.insert(std::make_pair(particle, std::make_pair(randomLifetime, 0.0f)));
					}
				}
				else
				{
					std::uniform_int_distribution int_dist(burst.minCount, burst.maxCount);
					int randomCount = int_dist(gen);
					for (int i = 0; i < randomCount; ++i)
					{
						// 파티클 풀에서 가져오기
						HDData::Particle* particle = HDEngine::ParticlePool::Instance().SummonParticle();
						// 파티클 정보 세팅
						particle->SetMesh(rendererModule.mesh);
						particle->SetMaterial(rendererModule.material);
						particle->SetRenderMode(rendererModule.renderMode);
						std::uniform_int_distribution color_dist_r(main.minStartColor.x, main.maxStartColor.x);
						std::uniform_int_distribution color_dist_g(main.minStartColor.y, main.maxStartColor.y);
						std::uniform_int_distribution color_dist_b(main.minStartColor.z, main.maxStartColor.z);
						std::uniform_int_distribution color_dist_a(main.minStartColor.w, main.maxStartColor.w);
						int r = color_dist_r(gen);
						int g = color_dist_g(gen);
						int b = color_dist_b(gen);
						int a = color_dist_a(gen);
						particle->SetColor(DirectX::XMINT4{ r, g, b, a });
						std::uniform_real_distribution speed_dist(main.minStartSpeed, main.maxStartSpeed);
						float speed = speed_dist(gen);
						particle->SetSpeed(speed);
						std::uniform_real_distribution size_dist(main.minStartSize, main.maxStartSize);
						float size = size_dist(gen);
						particle->SetSize(size);
						std::uniform_real_distribution rotation_dist(main.minStartRotation, main.maxStartRotation);
						float angle = rotation_dist(gen);
						particle->SetAngle(angle);
						float randomLifetime = real_dist(gen);
						_activatedParticles.insert(std::make_pair(particle, std::make_pair(randomLifetime, 0.0f)));
					}
				}
				++(burst.currentCycleCount);
			}
		}

		// 활성화 되어있는 파티클들 정보 업데이트
		for (auto& p : _activatedParticles)
		{
			if (colorOverLifetime.enabled)
			{
				p.first->SetColor(colorOverLifetime.color.Evaluate(_accumulatedDeltaTime));
			}
			if (sizeOverLifetime.enabled)
			{
				p.first->SetSize(sizeOverLifetime.size.Evaluate(_accumulatedDeltaTime / main.duration));
			}
			if (rotationOverLifetime.enabled)
			{
				p.first->SetAngle(p.first->GetAngle() + rotationOverLifetime.angularVelocity.Evaluate(_accumulatedDeltaTime / main.duration) * deltaTime);
			}

			// 파티클의 라이프타임과 누적 델타타임을 비교해서 시간이 다 된 파티클을 지워준다
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
				HDEngine::ParticlePool::Instance().Retrieve(iter->first);
				_activatedParticles.erase(iter);
			}
		}
		_lifeOverParticles.clear();

		if (_accumulatedDeltaTime > main.duration)
		{
			if (main.loop)
			{
				_accumulatedDeltaTime = 0.0f;
			}
			else
			{
				_isPlaying = false;
				// 활성화 되어 있던 파티클 삭제
				for (auto iter = _activatedParticles.begin(); iter != _activatedParticles.end(); ++iter)
				{
					HDEngine::ParticlePool::Instance().Retrieve(iter->first);
				}
				_activatedParticles.clear();
			}
		}
		else
		{
			_accumulatedDeltaTime += deltaTime;
		}
	}

}
