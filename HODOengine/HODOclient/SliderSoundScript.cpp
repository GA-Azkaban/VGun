#include "SliderSoundScript.h"

SliderSoundScript::SliderSoundScript()
	: _isSettingOn(true),
	_soundVolume(50)
{

}

SliderSoundScript::~SliderSoundScript()
{

}

void SliderSoundScript::Start()
{
	_audio = GetGameObject()->GetComponent<HDData::AudioSource>();
	_slider = GetGameObject()->GetComponent<HDData::SliderUI>();
}

void SliderSoundScript::Update()
{
	if (_isSettingOn == true)
	{
		_soundVolume = _slider->GetValue();
		_audio->SetSoundVolumeAll(_soundVolume);
	}
}
