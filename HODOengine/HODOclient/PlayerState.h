#pragma once


enum class ePlayerState
{
	NONE,
	IDLE,
	WALK,
	RUN,
	JUMP,
	CROUCH,
	CROUCH_WALK,
	RELOAD,
	FIRE,
	DIE,

	LAST
};

enum class eTeam
{
	R = 1,
	G = 2,
	B = 3
};
