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
	//서버에서 게임 허가 떨어졌을 때
	void InitGame();

	// 로비에서 데이터를 가져옴
	void GetNewDataFromLobby();
	
	// 값들을 재정비
	void InitializeValue();
	
	// 라운드 시작할 때 호출
	void InitRound();

	// 라운드 업데이트
	void UpdateRound();

	// 라운드 끝났을 때 호출
	void EndGame();
	void SetUIActive(bool isActive);
	void CheckWinner();

	// 방 나갈 때 호출
	void SetUIOrigin();
	void ExitGame();


public:
	bool CheckHeadColliderOwner(HDData::DynamicSphereCollider* collider);
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

	bool GetIsRoundStart();
	void SetIsRoundStart(bool isStart);
	void SetEndCam(HDData::GameObject* cam);
	void SetStartCam(HDData::Camera* cam);

	HDData::GameObject* GetEndCam();
	void SetRoundEndButton(HDData::GameObject* obj);
	HDData::GameObject* GetRoundEndButton();
	void SetWinnerText(HDData::TextUI* txt);
	void SetWinnerImg(HDData::ImageUI* img);
	void SetLoserText(HDData::TextUI* txt, int index);
	void SetLoserImg(HDData::ImageUI* img);

	bool GetMenuStatus();
	bool _ESCMenuOn = false;

	LowHPEffect* lowHPEffect;
	HDData::ImageUI* finRoundimg;
	HDData::ImageUI* startRoundimg;

	HDData::ImageUI* tumbleImage;
	HDData::ImageUI* tumbleAlphaImage;
	HDData::TextUI* tumbleCountText;

	HDData::ImageUI* hpImage;
	HDData::ImageUI* ammoImage;

	HDData::GameObject* crosshair;
	HDData::ImageUI* defaultCrosshair;
	HDData::ImageUI* hitCrosshair;
	HDData::ImageUI* criticalCrosshair;

private:
	bool _isRoundStart = false;
	HDData::Camera* _startCam;
	HDData::GameObject* _endCam;
	HDData::GameObject* _endObj;
	HDData::TextUI* _winnerTXT;
	HDData::ImageUI* _winnerImg;
	HDData::TextUI* _loserTXT[5];
	HDData::ImageUI* _loserImg;

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
	void StartSerialKillTimer();


private:
	HDData::TextUI* _timerUI;
	int _timer;

	Timer* _initTimer;
	Timer* _gameEndTimer;
	Timer* _showResultTimer;
	Timer* _serialKillTimer;
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

public:
	// obj 보관용
	std::pair<HDData::TextUI*, HDData::TextUI*> _killCountObjs[6];

public:
	// 인게임
	std::pair<HDData::TextUI*, HDData::TextUI*> _myKillCount;
	std::unordered_map<int, std::pair<HDData::TextUI*, HDData::TextUI*>> _inGameKillCounts;

	// 스폰 지점
	Vector3 _spawnPoint[15];
	int _index = 0;

	// 킬 카운트
	int _desiredKill;
	int _nowMaxKill;
	int _winnerUID;

public:
	void SetAnimationDummy(HDData::GameObject* obj);
	HDData::GameObject* GetAnimationDummy();
	void SetWeedColVector(std::vector<HDData::DynamicSphereCollider*>& vec);
	void ResetWeedPos();

private:
	HDData::GameObject* _animationDummy = nullptr;
	std::vector<HDData::DynamicSphereCollider*> _weedColVector;
};

