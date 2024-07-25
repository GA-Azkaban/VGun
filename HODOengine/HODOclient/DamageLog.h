#pragma once
#include "../HODOengine/HODO_API.h"
#include <array>
#include "Timer.h"

class DamageLog : public HDData::Script
{
	//////////////////////////////////////////////
	// Nickname		Damage		RemainHPPercent //
	// Nickname		Damage		RemainHPPercent //
	// Nickname		Damage		RemainHPPercent //
	// Nickname		Damage		RemainHPPercent //
	//////////////////////////////////////////////
public:
	enum class DamageLogType
	{
		Nickname,
		Damage,
		RemainHPPercent,
	};

public:
	DamageLog();

	void DisplayLog(const std::string& nickname, int damage, int remainHP);
	void SetActive(bool isActive);

protected:
	virtual void Start() override;
	virtual void Update() override;

private:
	constexpr static int MAXLOGCOUNT = 3;
	std::array<HDData::TextUI*, MAXLOGCOUNT> _nicknames;
	std::array<HDData::TextUI*, MAXLOGCOUNT> _damages;
	std::array<HDData::TextUI*, MAXLOGCOUNT> _remainHPs;

	std::array<float, 3> _uiXPosition;
	std::array<float, MAXLOGCOUNT> _uiYPosition;

	std::array<Timer, MAXLOGCOUNT> _logTimer;

	int _logIndex;

	HDData::AnimationCurve _curve;
};
