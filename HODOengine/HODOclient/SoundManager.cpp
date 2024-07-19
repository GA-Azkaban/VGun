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

}

SoundManager::~SoundManager()
{

}

void SoundManager::Start()
{
	_source = _audioController->AddComponent<HDData::AudioSource>();
}

void SoundManager::Update()
{

}

void SoundManager::InitializeAudios()
{
	// BGM
	_source->AddAudio("bgm_ingame", "Resources/Sound/BGM/INGAME.mp3", HDData::SoundGroup::BackgroundMusic);
	_source->AddAudio("bgm_lobby", "Resources/Sound/BGM/LOBBY.mp3", HDData::SoundGroup::BackgroundMusic);
}

void SoundManager::CheckCurrentSceneBGM()
{
	auto sceneName = API::GetCurrentSceneName();

	//if(sceneName == "InGame")
}
