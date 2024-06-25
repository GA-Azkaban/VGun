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

	void RecvOtherPlayerShoot(eHITLOC location);

	void SendJump(int uid);

private:
	void SetTeamColor(HDData::SkinnedMeshRenderer* mesh, eTeam color);

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

private:
	HDData::TextUI* _timerUI;
	int _timer;			// 타이머

	std::chrono::time_point<std::chrono::steady_clock> _start_time;

public:
	void SetDesiredKill(int count);
	int& GetDesiredKill();
	
	void SetMyKillCountUI(HDData::TextUI* txt);
	void SetOthersKillCount(HDData::TextUI* txt, int index);

private:
	HDData::TextUI* _myKillCount;
	HDData::TextUI* _othersKillCount[5];
	int _desiredKill;	// 목표 킬수

public:
	void SetAnimationDummy(HDData::GameObject* obj);
	HDData::GameObject* GetAnimationDummy();

private:
	HDData::GameObject* _animationDummy = nullptr;
};

