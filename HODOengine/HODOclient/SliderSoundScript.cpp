#include "SliderSoundScript.h"

SliderSoundScript::SliderSoundScript()
	: _isSettingOn(true),
	_soundVolume(0.5f)
{

}

SliderSoundScript::~SliderSoundScript()
{

}

void SliderSoundScript::Start()
{
	_audio = GetGameObject()->GetComponent<HDData::AudioSource>();
	_audio->AddAudio("ingame_bgm", "../Sound/BGM/ingame_bgm.wav", HDData::SoundGroup::BackgroundMusic);
	_audio->PlayRepeat("ingame_bgm");
	_slider = GetGameObject()->GetComponent<HDData::SliderUI>();
	_slider->SetDefaultValue(_soundVolume);
}

void SliderSoundScript::Update()
{
	if (_isSettingOn == true)
	{
		_soundVolume = _slider->GetValue() * 0.01f;
		_audio->SetSoundVolumeAll(_soundVolume);
	}
}
