﻿#pragma once

#include "../HODOengine/HODO_API.h"


class FPAniScript : public HDData::Script
{
public:
	FPAniScript();

public:
	void Start() override;
	void Update() override;

public:
	HDData::Animator* _animator;
};
