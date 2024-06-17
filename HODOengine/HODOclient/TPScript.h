#pragma once
#include "../HODOengine/HODO_API.h"

class TPScript : public HDData::Script
{
public:
	TPScript();

public:
	void Start() override;
	void Update() override;

private:
	HDData::Animator* _animator;

};

