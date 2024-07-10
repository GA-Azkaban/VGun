#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"
#include "IndicatorPool.h"
#include <vector>

class IndicatorTest : public HDData::Script
{
public:
	virtual void Update() override
	{
		if (API::GetKeyDown(DIK_M))
		{
			IndicatorPool::Instance().SummonIndicator({ -20, 0, 0 });
		}
	}
};
