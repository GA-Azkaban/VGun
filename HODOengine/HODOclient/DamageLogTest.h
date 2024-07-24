#pragma once
#include "../HODOengine/HODO_API.h"
#include "DamageLog.h"

class DamageLogTest : public HDData::Script
{
public:
	virtual void Update() override
	{
		if (API::GetKeyDown(DIK_T))
		{
			_damageLog->DisplayLog("Nickname", 33, 67);
		}
	}

	DamageLog* _damageLog;
};
