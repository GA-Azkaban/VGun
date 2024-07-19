#pragma once
#include <unordered_map>
#include <chrono>
#include <thread>

#include "../HODOengine/HODO_API.h"
#include "PlayerInfo.h"
#include "UIEffect.h"
#include "Timer.h"

class LowHPEffect;

class RoundManager : public HDData::Script
{
public:
	static RoundManager* Instance();

private:
	static RoundManager* _instance;
	RoundManager();
	~RoundManager() = default;

	HDData::Scene* _roundScene = nullptr;

public:
	void Start() override;
	void Update() override;

	void SetRoundScene(HDData::Scene* scene);

public:
	void InitGame();
	void EndGame();

public:
	void InitRound();
	void UpdateRound();
	void SetUIActive(bool isActive);


public:
	void CheckHeadColliderOwner(HDData::DynamicSphereCollider* collider);
	void CheckBodyColliderOwner(HDData::DynamicCapsuleCollider* collider);

	void SendJump(int uid);

public:
	std::unordered_map<int, HDData::GameObject*>& GetPlayerObjs();
	int GetPlayerNum();

	HDData::GameObject* _myObj;
	std::vector<HDData::GameObject*> _playerObjs;

private:
	int _playerNum;
	std::unordered_map<int, HDData::GameObject*> _players;

public:
	void CheckWinner();

	bool GetIsRoundStart();
	void SetIsRoundStart(bool isStart);
	void SetEndCam(HDData::GameObject* cam);
	void SetStartCam(HDData::Camera* cam);
	
	HDData::GameObject* GetEndCam();
	void SetRoundEndButton(HDData::GameObject* obj);
	HDData::GameObject* GetRoundEndButton();
	void ExitGame();
	void SetWinnerText(HDData::TextUI* txt);
	void SetLoserText(HDData::TextUI* txt, int index);

	bool GetMenuStatus();
	bool _ESCMenuOn = false;

	LowHPEffect* lowHPEffect;
	HDData::ImageUI* finRoundimg;
	HDData::ImageUI* startRoundimg;

	HDData::ImageUI* tumbleImage;
	HDData::ImageUI* tumbleAlphaImage;
	HDData::TextUI* tumbleCountText;

private:
	bool _isRoundStart = false;
	HDData::Camera* _startCam;
	HDData::GameObject* _endCam;
	HDData::GameObject* _endObj;
	HDData::TextUI* _winnerTXT;
	HDData::TextUI* _loserTXT[5];

public:
	void SetRoundTimerObject(HDData::TextUI* obj);
	void SetRoundTimer(int time);
	void SetStartTime(std::chrono::time_point<std::chrono::steady_clock> time);
	int& GetRoundTimer();
	void UpdateRoundTimer();
	std::string ChangeSecToMin(int second);
	void SetHPObject(HDData::TextUI* txt);
	void UpdateHPText();
	void SetAmmoText(HDData::TextUI* txt);
	void UpdateAmmoText();
	void UpdateBeginEndTimer();
	void SetResultTimerUI(HDData::TextUI* txt);
	Timer* GetGameEndTimer();
	void SetInitRoundTimer(HDData::TextUI* txt);
	

private:
	HDData::TextUI* _timerUI;
	int _timer;			

	Timer* _initTimer;
	Timer* _gameEndTimer;
	Timer* _showResultTimer;
	std::chrono::time_point<std::chrono::steady_clock> _start_time;

	HDData::TextUI* _hpUI;
	HDData::TextUI* _ammoUI;
	HDData::TextUI* _resultTimerUI;
	HDData::TextUI* _initTimertxt;

public:
	void UpdateDesiredKillChecker();
	void SetDesiredKill(int count);
	int& GetDesiredKill();
	void SetKillCountUI(HDData::TextUI* nick, HDData::TextUI* count, int index);
	//void SetKillCountBack(HDData::ImageUI* img, int index);
	std::unordered_map<int, std::pair<HDData::TextUI*, HDData::TextUI*>>& GetKillCountMap();

private:
	// obj 보관용
	std::pair<HDData::TextUI*, HDData::TextUI*> _killCountObjs[6];
	// 인게임
	std::unordered_map<int, std::pair<HDData::TextUI*, HDData::TextUI*>> _inGameKillCounts;

	// 스폰 지점
	Vector3 _spawnPoint[15];
	int _index = 0;

	// 킬 카운트
	int _desiredKill;
	int _nowMaxKill;
	int _winnerUID;

	// UI
	HDData::ImageUI* _backIMG[6];

public:
	void SetAnimationDummy(HDData::GameObject* obj);
	HDData::GameObject* GetAnimationDummy();

private:
	HDData::GameObject* _animationDummy = nullptr;

};

