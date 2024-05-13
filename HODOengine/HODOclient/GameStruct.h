#pragma once

struct roominfo
{
	int id;
	std::string title;
	int maxPlayerCount;
	int currentPlayerCount;

	bool isPrivate;
	bool isTeam;
};
