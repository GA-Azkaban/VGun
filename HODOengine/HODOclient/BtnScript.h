﻿#pragma once

#include "..\HODOengine\HODO_API.h"
namespace HDData
{
	class GameObject;
}

class BtnScript : public HDData::Script
{
public:
	BtnScript() = default;
	BtnScript(std::string Name);
	virtual ~BtnScript();

	virtual void Start() override;
	virtual void Update() override;

	std::string imgName;

	HDData::GameObject* _childObject;

};

