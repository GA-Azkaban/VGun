#pragma once
#include <unordered_map>

#include "../HODOengine/HODO_API.h"
#include "Weapon.h"

class GameManager : public HDData::Script
{
public:
	GameManager* GetInstance();

private:
	static GameManager* instance;
	GameManager();
	~GameManager() = default;

public:
	void Start() override;
	void Update() override;

public:
	void InitGame();
	void EndGame();

public:
	void InitRound();
	void UpdateRound();
};

