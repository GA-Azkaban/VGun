#pragma once
#include <unordered_map>

#include "../HODOengine/HODO_API.h"

#include "GameSetting.h"

/// <summary>
/// 게임의 사운드들을 제어하는 시스템
/// BGM, 나 자신의 사운드는 2D로 관리하면 된다
/// 아싸리 게임매니저에서 내 상태를 들고 있으면 되지 않을까?
/// 
/// 3D사운드는 상대방에게 갖다 붙여야 함
/// 라운
/// </summary>

enum class eBGM
{
	LOBBY,
	INGAME,

	LAST = 99
};

enum class eSFX
{
	CLICK,
	TIMER,
	WIND,
	WHISTLE,

	WALK,
	FIRE,
	RELOAD,
	ROLL,
	JUMP,
	ATTACKED,
	DIE,
	RESPAWN,


	LAST = 99
};

class SoundManager : public HDData::Script
{
public:
	static SoundManager& Instance();

private:
	static SoundManager* _instance;
	SoundManager();
	~SoundManager();

public:
	void Start() override;
	void Update() override;

public:
	void SetAllVolume(float vol);
	void SetBGMVolme(float vol);
	void SetSFXVolume(float vol);

public:
	void InitializeAudios();
	void InitializePlayerAudio(HDData::AudioSource* audio);

public:
	void PlayBGM(std::string sourceName);
	void PlayUI(std::string souceName);

private:
	// 현재 씬이 바뀔 때마다 씬을 체크하고 BGM 변경
	void CheckSceneBGM();
	void UpdateBGMVolume();
	void UpdateSFXVolume();

private:
	std::string _currentSceneName;
	float _volume;

public:
	HDData::AudioSource* AddAudioSourceInObject(HDData::GameObject* obj);

private:
	HDData::GameObject* _audioController;
	HDData::AudioSource* _2DsoundController;
	HDData::GameObject* _UIaudioController;
	HDData::AudioSource* _UISoundController;


	std::vector<HDData::AudioSource*> _sources;

};

