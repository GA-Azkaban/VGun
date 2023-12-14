#pragma once
#include "../HODOengine/HODO_API.h"

class PlayerMove : public HDData::Script
{
public:
	PlayerMove();

public:
	void Start() override;
	void Update() override;

};
