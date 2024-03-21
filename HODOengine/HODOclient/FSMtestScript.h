#pragma once
#include "../HODOengine/HODO_API.h"

class FSMtestScript : public HDData::Script
{
public:
	FSMtestScript();

	enum class ePlayerState
	{
		IDLE,
		WALK,
		JUMP,
		SIT,

		LAST
	};

public:
	void Start() override;
	void Update() override;

public:
	HDData::Animator* _animator;

};

