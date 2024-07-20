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
	_2DsoundController = _audioController->AddComponent<HDData::AudioSource>();
	_currentSceneName = API::GetCurrentSceneName();
	InitializeAudios();
	PlayBGM("bgm_lobby");
}

void SoundManager::Update()
{
	CheckSceneBGM();
	CheckUIClicked();
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
	_2DsoundController->StopAll();
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

void SoundManager::PlayUI(std::string souceName)
{
	_2DsoundController->PlayOnce(souceName);
}

void SoundManager::CheckUIClicked()
{

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
	_2DsoundController->AddAudio("sfx_button", "Resources/Sound/BGM/BUTTON.mp3", HDData::SoundGroup::EffectSound);
	_2DsoundController->AddAudio("sfx_entry", "Resources/Sound/BGM/ENTRY.mp3", HDData::SoundGroup::EffectSound);

	// SFX

}
