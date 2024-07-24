#pragma once

enum class ePlayerMoveState
{
	NONE,

	// first
	IDLE,
	WALK,
	RUN,
	JUMP,
	TUMBLE,
	DIE,

	//second
	AIM,
	FIRE,
	EMPTY,
	RELOAD
};
