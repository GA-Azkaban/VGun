#pragma once


enum class ePlayerState
{
	NONE,
	IDLE,
	FIRE,
	WALK_R,
	WALK_L,
	WALK_F,
	WALK_B,
	WALK,
	RUN,
	JUMP,
	RELOAD,
	ROLL,
	ROLL_R,
	ROLL_L,
	ROLL_F,
	ROLL_B,
	DIE,
	TUMBLE,

	LAST
};

enum class eTeam
{
	R = 1,
	G = 2,
	B = 3
};
