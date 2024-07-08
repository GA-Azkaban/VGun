#include "IndicatorPool.h"

IndicatorPool* IndicatorPool::_instance = nullptr;

IndicatorPool::IndicatorPool()
{
	for(int i = 0; i < 10; ++i)
	{
		auto indicator = Indicator::CreateIndicator();
		_indicatorList.push_back(indicator);
	}
}

IndicatorPool::~IndicatorPool()
{
	for(auto indicator : _indicatorList)
	{
		delete indicator;
		indicator = nullptr;
	}
	_indicatorList.clear();
}

IndicatorPool& IndicatorPool::Instance()
{
	if(_instance == nullptr)
	{
		_instance = new IndicatorPool();
	}
	return *_instance;
}

Indicator* IndicatorPool::SummonIndicator(Vector3 targetPos)
{
	if(_indicatorList.empty())
	{
		auto indicator = Indicator::CreateIndicator();
		_indicatorList.push_back(indicator);		
	}

	Indicator* indicator = _indicatorList.back();
	indicator->GetGameObject()->SetSelfActive(true);
	indicator->SetEffectOn(targetPos);
	_indicatorList.pop_back();
	return indicator;
}

void IndicatorPool::RetrieveIndicator(Indicator* indicator)
{
	indicator->GetGameObject()->SetSelfActive(false);
	_indicatorList.push_back(indicator);
}

