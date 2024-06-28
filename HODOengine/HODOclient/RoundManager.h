#pragma once
#include <unordered_map>
#include <chrono>
#include <thread>

#include "../HODOengine/HODO_API.h"
#include "PlayerInfo.h"

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
	bool GetIsRoundStart();
	void SetIsRoundStart(bool isStart);

private:
	bool _isRoundStart = false;

public:
	void SetRoundTimerObject(HDData::TextUI* obj);
	void SetRoundTimer(int time);
	void SetStartTime(std::chrono::time_point<std::chrono::steady_clock> time);
	int& GetRoundTimer();
	void UpdateRoundTimer();
	std::string ChangeSecToMin(int second);
	void SetHPObject(HDData::TextUI* txt);
	void UpdateHPText();

private:
	HDData::TextUI* _timerUI;
	int _timer;			

	std::chrono::time_point<std::chrono::steady_clock> _start_time;

	HDData::TextUI* _hpUI;

public:
	void UpdateDesiredKillChecker();
	void SetDesiredKill(int count);
	int& GetDesiredKill();
	void SetKillCountUI(HDData::TextUI* nick, HDData::TextUI* count, int index);
	
	std::unordered_map<int, std::pair<HDData::TextUI*, HDData::TextUI*>>& GetKillCountMap();



private:
	// obj 보관용
	std::pair<HDData::TextUI*, HDData::TextUI*> _killCountObjs[6];

	// 스폰 지점
	Vector3 _spawnPoint[10];
	void SetSpawnPoint();

	// 인게임
	std::unordered_map<int, std::pair<HDData::TextUI*, HDData::TextUI*>> _inGameKillCounts;

	int _desiredKill;	// 목표 킬수
	int _winnerUID;

public:
	void SetAnimationDummy(HDData::GameObject* obj);
	HDData::GameObject* GetAnimationDummy();

private:
	HDData::GameObject* _animationDummy = nullptr;
};

