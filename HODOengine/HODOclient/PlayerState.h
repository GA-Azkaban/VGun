#pragma once


enum class ePlayerState
{
	NONE,
	IDLE,
	WALK_R,
	WALK_L,
	WALK_F,
	WALK_B,
	JUMP,
	RELOAD,
	ROLL,
	DIE,

	LAST
};

enum class eTeam
{
	R = 1,
	G = 2,
	B = 3
};
