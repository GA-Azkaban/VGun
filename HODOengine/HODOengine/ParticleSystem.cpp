#include "ParticleSystem.h"
#include "GameObject.h"
#include "ParticleSystemRenderer.h"
#include "TimeSystem.h"
#include "ParticlePool.h"

namespace HDData
{

	ParticleSystem::ParticleSystem()
		: main(), colorOverLifetime(), emission(),
		limitVelocityOverLifetime(), sizeOverLifetime(), rotationOverLifetime(),
		useAutoRandomSeed(true), particleCount(0), time(0.0f),
		_isPlaying(false), _accumulatedDeltaTime(0.0f)
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
		if (_accumulatedDeltaTime >= emission.GetBurst().time)
		{
			if (emission.GetBurst().currentCycleCount < emission.GetBurst().cycleCount)
			{
				if (emission.GetBurst().count > -1)
				{
					for (int i = 0; i < emission.GetBurst().count; ++i)
					{
						// 파티쿨 풀에서 가져오기
						HDEngine::IParticle* particle = HDEngine::ParticlePool::Instance().SummonParticle();
						// 파티클 정보 세팅

						_activatedParticles.insert(particle);
					}
				}
				else
				{
					int randomCount = 
					for (int i = 0; i < emission.GetBurst().count; ++i)
					{
						// 파티쿨 풀에서 가져오기
						HDEngine::IParticle* particle = HDEngine::ParticlePool::Instance().SummonParticle();
						// 파티클 정보 세팅

						_activatedParticles.insert(particle);
					}
				}
				++(emission.GetBurst().currentCycleCount);
			}
		}

		// 활성화 되어있는 파티클들 정보 업데이트


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
					HDEngine::ParticlePool::Instance().Retrieve(*iter);
				}
				_activatedParticles.clear();
			}
		}

		_accumulatedDeltaTime += HDEngine::TimeSystem::Instance().GetDeltaTime();
		particleCount = _activatedParticles.size();
	}

}
