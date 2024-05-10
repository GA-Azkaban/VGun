#pragma once

struct roominfo
{
	int id;
	std::string name;
	int maxPlayerCount;
	int currentPlayerCount;

	bool isPrivate;
	bool isTeam;
};
