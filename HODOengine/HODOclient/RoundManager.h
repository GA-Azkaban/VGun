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

private:
	bool _isRoundStart = false;

	int _timer;			// 타이머

public:
	bool GetIsRoundStart();
	void SetIsRoundStart(bool isStart);

	//애니메이션

public:
	void SetAnimationDummy(HDData::GameObject* obj);
	HDData::GameObject* GetAnimationDummy();

private:
	HDData::GameObject* _animationDummy = nullptr;
};

