#pragma once
#include <unordered_map>

#include "../HODOengine/HODO_API.h"
#include "Weapon.h"
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

public:
	std::vector<HDData::GameObject*>& GetPlayerObjs();
	int GetPlayerNum();

private:
	int _playerNum;
	std::vector<HDData::GameObject*> _playerObjs;

	// 해시맵으로 변경 중
private:
	int _playerCount;
	std::unordered_map<int, HDData::GameObject*> _players;

private:
	bool _isRoundStart = false;

	 
	// 라운드 정보
	int _timer;			// 타이머

	int _R;				// 팀당 킬카운트
	int _G;
	int _B;

public:
	void SetIsRoundStart(bool isStart);

private:
	//std::vector<HDData::GameObject*> _randomSpawnPos;
};

