#pragma once
#include "../HODOengine/HODO_API.h"
#include "../HODOengine/AudioSource.h"

#define FILEPATH "Resources/Sound/"

class SoundPlayer : public HDData::Script
{
public:
	virtual void Start() override
	{
		_audioSource = GetGameObject()->GetComponent<HDData::AudioSource>();

		_audioSource->AddAudio("lobby_bgm","Resources/Sound/BGM/Sample_BGM.mp3",HDData::SoundGroup::BackgroundMusic);
	}

	virtual void Update() override
	{
		_audioSource->PlayRepeat("lobby_bgm");
	}

	void PlayBGM()
	{
		_audioSource->PlayRepeat("lobby_bgm");
	}

private:
	HDData::AudioSource* _audioSource;
};
