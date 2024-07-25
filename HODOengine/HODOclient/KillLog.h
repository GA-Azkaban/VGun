#pragma once
#include "../HODOengine/HODO_API.h"
#include <array>
#include "Timer.h"

class KillLog : public HDData::Script
{
	///////////////////////////////////////////////
	// Nickname			kill			Nickname //
	// Nickname			kill			Nickname //
	// Nickname			kill			Nickname //
	// Nickname			kill			Nickname //
	///////////////////////////////////////////////
public:
	enum class KillLogType
	{
		PLAYERKILLENEMY,
		ENEMYKILLPLAYER,
		ENEMYKILLENEMY,
	};

public:
	KillLog();

	void DisplayLog(const std::string& nickname, const std::string& deadNickname, KillLogType logType);
	void SetActive(bool isActive);

protected:
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnDisable() override;

private:
	constexpr static int MAXLOGCOUNT = 4;
	std::array<HDData::TextUI*, MAXLOGCOUNT> _killerNicknames;
	std::array<HDData::TextUI*, MAXLOGCOUNT> _kills;
	std::array<HDData::TextUI*, MAXLOGCOUNT> _deadNicknames;

	std::array<float, 3> _uiXPosition;
	std::array<float, MAXLOGCOUNT> _uiYPosition;

	std::array<Timer, MAXLOGCOUNT> _logTimer;

	int _logIndex;

	HDData::AnimationCurve _curve;
};
