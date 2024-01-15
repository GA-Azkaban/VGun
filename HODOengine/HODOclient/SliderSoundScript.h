#pragma once
#include "../HODOengine/HODO_API.h"

class SliderSoundScript : public HDData::Script
{
public:
	SliderSoundScript();
	~SliderSoundScript();

	void Start() override;
	void Update() override;

private:
	bool _isSettingOn;
	int _soundVolume;

	HDData::AudioSource* _audio;
	HDData::SliderUI* _slider;
};

