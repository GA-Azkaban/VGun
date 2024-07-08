#pragma once
#include "../HODOengine/HODO_API.h"
#include "Indicator.h"
#include <vector>

class IndicatorPool
{
public:
	~IndicatorPool();

	static IndicatorPool& Instance();

	Indicator* SummonIndicator(Vector3 targetPos);
	void RetrieveIndicator(Indicator* indicator);

	HDData::GameObject* player;

private:
	IndicatorPool();
	static IndicatorPool* _instance;
	std::vector<Indicator*> _indicatorList;
};
