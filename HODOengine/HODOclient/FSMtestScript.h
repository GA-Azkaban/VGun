#pragma once
#include "../HODOengine/HODO_API.h"

class FSMtestScript : public HDData::Script
{
public:
	FSMtestScript();

	enum class ePlayerState
	{
		IDLE,
		AIM_IDLE,
		SHOOT_IDLE,
		WALK,
		AIM_WALK,
		SHOOT_WALK,
		RUN,
		JUMP,
		AIM_JUMP,
		CRUNCH,
		AIM_CRUNCH,
		SHOOT_CRUNCH,

		LAST
	};

public:
	void Start() override;
	void Update() override;

public:
	HDData::Animator* _animator;
	ePlayerState _currentState;

};

