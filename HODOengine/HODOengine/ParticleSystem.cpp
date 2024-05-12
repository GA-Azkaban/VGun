﻿#include "ParticleSystem.h"
#include "GameObject.h"
#include "ParticleSystemRenderer.h"
#include "TimeSystem.h"
#include "ParticlePool.h"

namespace HDData
{

	ParticleSystem::ParticleSystem()
		: main(), colorOverLifetime(), emission(),
		limitVelocityOverLifetime(), sizeOverLifetime(), rotationOverLifetime(),
		useAutoRandomSeed(true),time(0.0f),
		_isPlaying(false), _accumulatedDeltaTime(0.0f),
		rd(), gen(rd)
	{
		_renderer = GetGameObject()->AddComponent<ParticleSystemRenderer>();
	}

	ParticleSystem::~ParticleSystem()
	{

	}

	void ParticleSystem::Play()
	{
		_isPlaying = true;
		_accumulatedDeltaTime = 0.0f;
		emission.GetBurst().currentCycleCount = 0;
	}

	void ParticleSystem::Stop()
	{
		_isPlaying = false;
	}

	void ParticleSystem::Clear()
	{
		_activatedParticles.clear();
	}

	void ParticleSystem::Update()
	{
		if (!_isPlaying)
		{
			return;
		}

		// emission의 지정된 시간에 지정된 개수만큼 파티클 생성
		// 파티클은 풀에서 꺼내온다
		Burst& burst = emission.GetBurst();
		if (_accumulatedDeltaTime >= burst.time)
		{
			if (burst.currentCycleCount < burst.cycleCount)
			{
				std::uniform_real_distribution distribution(main.minStartLifetime, main.maxStartLifetime);
				if (burst.count > -1)
				{
					for (int i = 0; i < burst.count; ++i)
					{
						// 파티쿨 풀에서 가져오기
						HDEngine::IParticle* particle = HDEngine::ParticlePool::Instance().SummonParticle();
						// 파티클 정보 세팅
						std::string mesh = _renderer->GetMesh();
						if (mesh != "")
						{
							particle->SetMesh(mesh);
						}
						particle->SetMaterial(_renderer->GetMaterial()->_material);
						particle->SetRenderMode(_renderer->GetRenderMode());
						float randomLifetime = distribution(gen);
						_activatedParticles.insert(std::make_pair(particle, std::make_pair(randomLifetime, 0.0f)));
					}
				}
				else
				{
					std::uniform_int_distribution dist(burst.minCount, burst.maxCount);
					int randomCount = dist(gen);
					for (int i = 0; i < randomCount; ++i)
					{
						// 파티클 풀에서 가져오기
						HDEngine::IParticle* particle = HDEngine::ParticlePool::Instance().SummonParticle();
						// 파티클 정보 세팅
						std::string mesh = _renderer->GetMesh();
						if (mesh != "")
						{
							particle->SetMesh(mesh);
						}
						particle->SetMaterial(_renderer->GetMaterial()->_material);
						particle->SetRenderMode(_renderer->GetRenderMode());
						float randomLifetime = distribution(gen);
						_activatedParticles.insert(std::make_pair(particle, std::make_pair(randomLifetime, 0.0f)));
					}
				}
				++(burst.currentCycleCount);
			}
		}

		// 활성화 되어있는 파티클들 정보 업데이트
		for (auto& p : _activatedParticles)
		{
			p.first->SetColor(colorOverLifetime.color.Evaluate(_accumulatedDeltaTime));
			/// TODO) 파티클의 라이프타임과 누적 델타타임을 비교해서 시간이 다 된 파티클을 지워준다
			p.second.second += HDEngine::TimeSystem::Instance().GetDeltaTime();
		}

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

		_accumulatedDeltaTime += HDEngine::TimeSystem::Instance().GetDeltaTime();
	}

}
