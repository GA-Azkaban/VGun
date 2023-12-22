#pragma once
#include "../HODOengine/HODO_API.h"
#include "../HODOengine/AudioSource.h"

/// <summary>
/// AudioSource 컴포넌트와 SoundSystem 테스트를 위한 임시 스크립트.
/// 추후 삭제예정.
/// 게임오브젝트에 AudioSource 컴포넌트를 부착한 후 이것을 통해 사운드 추가, 사용한다.
/// TestScene의 Player 오브젝트에 AudioSource
/// </summary>
class TestSound : public HDData::Script
{
public:
	virtual void Start() override
	{
		_audioSource = GetGameObject()->GetComponent<HDData::AudioSource>();
		// 배경음
		_audioSource->AddAudio("ingame_bgm", "../Sound/BGM/ingame_bgm.wav", HDData::SoundGroup::BackgroundMusic);
		// 효과음1
		_audioSource->AddAudio3D("Gun_sound", "../Sound/Shoot/Gun_sound.wav", HDData::SoundGroup::EffectSound, 1, 10000);
		// 효과음2
		_audioSource->AddAudio3D("parts_buy", "../Sound/PartsBuy/parts_buy.wav", HDData::SoundGroup::EffectSound, 1, 10000);
	}
	virtual void Update() override
	{
		if (API::GetKeyDown('1'))
		{
			_audioSource->PlayRepeat("ingame_bgm");
		}
		if (API::GetKeyDown('2'))
		{
			_audioSource->Play3DOnce("Gun_sound", HDMath::HDFLOAT3{ 0,0,0 });
		}
		if (API::GetKeyDown('3'))
		{
			_audioSource->Play3DOnce("parts_buy", HDMath::HDFLOAT3{ 20,0,0 });
		}
		if (API::GetKeyDown('4'))
		{
			_audioSource->StopAll();
		}
	}

private:
	HDData::AudioSource* _audioSource;
};