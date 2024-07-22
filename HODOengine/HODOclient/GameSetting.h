#pragma once
#include "../HODOengine/HODO_API.h"

class GameSetting : public HDData::Script
{
public:
	static GameSetting& Instance();

private:
	static GameSetting* _instance;
	GameSetting();
	~GameSetting();

public:
	void Start() override;
	void Update() override;

public:
	void SetSettingCanvasActive(bool _isActive);

public:
	HDData::GameObject* _settingCanvas;
	HDData::GameObject* _bgmSoundController;
	HDData::GameObject* _sfxSoundController;

	HDData::GameObject* _bgmSoundIndex;
	HDData::GameObject* _bgmSoundSlider;

	HDData::GameObject* _sfxSoundIndex;
	HDData::GameObject* _sfxSoundSlider;
};

