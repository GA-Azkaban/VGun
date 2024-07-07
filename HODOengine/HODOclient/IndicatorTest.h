#pragma once
#include "../HODOengine/HODO_API.h"
#include "IndicatorPool.h"

class IndicatorTest : public HDData::Script
{
public:
	virtual void Update() override
	{
		if (API::GetKeyDown(DIK_1))
		{
			IndicatorPool::Instance().SummonIndicator({0, 0, 10});
		}
	}
};
