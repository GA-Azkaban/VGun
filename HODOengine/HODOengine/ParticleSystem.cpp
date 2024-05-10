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
				// 파티쿨 풀에서 가져오기
				HDEngine::IParticle* particle = HDEngine::ParticlePool::Instance().SummonParticle();

				++(emission.GetBurst().currentCycleCount);
			}
		}

		_accumulatedDeltaTime += HDEngine::TimeSystem::Instance().GetDeltaTime();

		if (_accumulatedDeltaTime > main.duration)
		{
			if (main.loop)
			{
				_accumulatedDeltaTime = 0.0f;
			}
			else
			{
				_isPlaying = false;
			}
		}
	}

}
