#pragma once
#include "../HODOengine/HODO_API.h"

/// <summary>
/// 게임의 사운드들을 제어하는 시스템
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

private:
	void InitializeAudios();
	void CheckCurrentSceneBGM();


private:
	HDData::GameObject* _audioController;
	HDData::AudioSource* _source;

};

