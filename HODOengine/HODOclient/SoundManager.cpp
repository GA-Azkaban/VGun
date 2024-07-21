#include "SoundManager.h"

#define BGM_PATH "Resources/Sound/BGM/"
#define SFX_PATH "Resources/Sound/SFX/"

SoundManager& SoundManager::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new SoundManager;
	}

	return *_instance;
}

SoundManager* SoundManager::_instance;

SoundManager::SoundManager()
{
	API::CreateStaticComponent(this);
}

SoundManager::~SoundManager()
{
	delete _audioController;
	delete _instance;
}

void SoundManager::Start()
{
	_audioController = API::CreateStaticObject();
	_UIaudioController = API::CreateStaticObject();
	_2DsoundController = _audioController->AddComponent<HDData::AudioSource>();
	_UISoundController = _UIaudioController->AddComponent<HDData::AudioSource>();
	_currentSceneName = API::GetCurrentSceneName();
	SetAllVolume(1);
	InitializeAudios();
	PlayBGM("bgm_lobby");
	//PlayUI("sfx_bell");
}

void SoundManager::Update()
{
	CheckSceneBGM();
	UpdateBGMVolume();
	UpdateSFXVolume();
	_sources;
}

void SoundManager::SetAllVolume(float vol)
{
	_2DsoundController->SetSoundVolumeAll(vol);

	for (auto& sound : _sources)
	{
		sound->SetSoundVolumeAll(vol);
	}
}

void SoundManager::SetBGMVolme(float vol)
{
	_2DsoundController->SetSoundGroupVolume(HDData::SoundGroup::BackgroundMusic, vol);

	for (auto& sound : _sources)
	{
		sound->SetSoundGroupVolume(HDData::SoundGroup::BackgroundMusic, vol);
	}
}

void SoundManager::SetSFXVolume(float vol)
{
	_2DsoundController->SetSoundGroupVolume(HDData::SoundGroup::EffectSound, vol);

	for (auto& sound : _sources)
	{
		sound->SetSoundGroupVolume(HDData::SoundGroup::EffectSound, vol);
	}
}

void SoundManager::PlayBGM(std::string sourceName)
{
	_2DsoundController->StopSoundGroup(HDData::SoundGroup::BackgroundMusic);
	_2DsoundController->PlayRepeat(sourceName);
}

void SoundManager::CheckSceneBGM()
{
	if (API::GetCurrentSceneName() != _currentSceneName)
	{
		auto sceneNow = API::GetCurrentSceneName();
		_currentSceneName = sceneNow;

		if (_2DsoundController->IsSoundPlaying("bgm_lobby"))
		{
			if (sceneNow == "InGame") PlayBGM("bgm_ingame");

		}
		else if (_2DsoundController->IsSoundPlaying("bgm_ingame"))
		{
			if (sceneNow != "InGame") PlayBGM("bgm_lobby");
		}
		else
		{
			if (sceneNow != "InGame") PlayBGM("bgm_lobby");
		}
	}
}

void SoundManager::UpdateBGMVolume()
{
	auto val = GameSetting::Instance()._bgmSoundSlider->GetComponent<HDData::SliderUI>()->GetValueText();
	SetBGMVolme(GameSetting::Instance()._bgmSoundSlider->GetComponent<HDData::SliderUI>()->GetValueText() * 0.01);
}

void SoundManager::UpdateSFXVolume()
{
	auto val = GameSetting::Instance()._sfxSoundSlider->GetComponent<HDData::SliderUI>()->GetValueText();
	SetSFXVolume(GameSetting::Instance()._sfxSoundSlider->GetComponent<HDData::SliderUI>()->GetValueText() * 0.01);
}

void SoundManager::PlayUI(std::string souceName)
{
	_UISoundController->PlayOnce(souceName);
}

HDData::AudioSource* SoundManager::AddAudioSourceInObject(HDData::GameObject* obj)
{
	auto source = obj->AddComponent<HDData::AudioSource>();
	_sources.push_back(source);
	return source;
}

void SoundManager::InitializeAudios()
{
	// BGM
	_2DsoundController->AddAudio("bgm_ingame", "Resources/Sound/BGM/INGAME.mp3", HDData::SoundGroup::BackgroundMusic);
	_2DsoundController->AddAudio("bgm_lobby", "Resources/Sound/BGM/LOBBY.mp3", HDData::SoundGroup::BackgroundMusic);
	_2DsoundController->AddAudio("bgm_victory", "Resources/Sound/BGM/VICTORY.mp3", HDData::SoundGroup::BackgroundMusic);
	_2DsoundController->AddAudio("bgm_defeat", "Resources/Sound/BGM/VICTORY.mp3", HDData::SoundGroup::BackgroundMusic);

	// UI
	_UISoundController->AddAudio("sfx_button", "Resources/Sound/SFX/BUTTON.wav", HDData::SoundGroup::EffectSound);
	_UISoundController->AddAudio("sfx_entry", "Resources/Sound/SFX/ENTRY.wav", HDData::SoundGroup::EffectSound);
	_UISoundController->AddAudio("sfx_bell", "Resources/Sound/SFX/BELL.mp3", HDData::SoundGroup::EffectSound);
	_UISoundController->AddAudio("sfx_timer", "Resources/Sound/SFX/BELL.mp3", HDData::SoundGroup::EffectSound);

}

void SoundManager::InitializePlayerAudio(HDData::AudioSource* audio)
{
	// 3d sound
	audio->AddAudio3D("3d_fire", "Resources/Sound/SFX/FIRE.wav", HDData::SoundGroup::EffectSound, 10, 100);
	audio->AddAudio3D("3d_footstep", "Resources/Sound/SFX/FOOTSTEP.mp3", HDData::SoundGroup::EffectSound, 10, 100);
	
	// 2d sound
	audio->AddAudio("2d_jump", "Resources/Sound/SFX/JUMP.wav", HDData::SoundGroup::EffectSound);
	audio->AddAudio("2d_roll", "Resources/Sound/SFX/TUMBLE.wav", HDData::SoundGroup::EffectSound);
	audio->AddAudio("2d_attacked", "Resources/Sound/SFX/ATTACKED.wav", HDData::SoundGroup::EffectSound);
	audio->AddAudio("2d_die", "Resources/Sound/SFX/DIE.wav", HDData::SoundGroup::EffectSound);
	audio->AddAudio("2d_reload", "Resources/Sound/SFX/RELOAD.wav", HDData::SoundGroup::EffectSound);
	audio->AddAudio("2d_fire", "Resources/Sound/SFX/FIRE.wav", HDData::SoundGroup::EffectSound);
	audio->AddAudio("2d_reload", "Resources/Sound/SFX/FOOTSTEP.mp3", HDData::SoundGroup::EffectSound);

}
