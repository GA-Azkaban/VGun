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
	CROUCH,
	CROUCH_R,
	CROUCH_L,
	CROUCH_F,
	CROUCH_B,
	RELOAD,
	DIE,

	LAST
};

enum class eTeam
{
	R = 1,
	G = 2,
	B = 3
};
