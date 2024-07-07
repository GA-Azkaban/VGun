#include "Indicator.h"
#include "IndicatorPool.h"
#include <cmath>
#include <numbers>

Indicator::Indicator()
{
	_indicatorTimer.isRepeat = false;
	_indicatorTimer.duration = 1.0f;
	_indicatorTimer.onExpiration = [&]() { IndicatorPool::Instance().RetrieveIndicator(this); };
}

float Indicator::CalculateDegreeBetweenTwoVectors(Vector3 v1, Vector3 v2)
{
	v1.Normalize();
	v2.Normalize();

	float scalar = v1.x * v2.x + v1.z * v2.z;
	float length1 = v1.Length();
	float length2 = v2.Length();
	float radian = acosf(scalar / (length1 * length2));

	Vector3 crossVec = v1.Cross(v2);
	//float mul = v1.x * v2.y - v1.y * v2.x;

	//if (mul * crossVec.z >= 0)
	if(crossVec.y >= 0)
	{
		float degree = radian * 180.0f / std::numbers::pi;
		return radian;
	}
	else
	{
		float degree = radian * 180.0f / std::numbers::pi;
		float resultRadian = (360.0f - degree) * std::numbers::pi / 180.0f;
		return resultRadian;
	}
}

Indicator* Indicator::CreateIndicator()
{
	auto obj = API::CreateObject(API::GetCurrenScene());
	auto comp = obj->AddComponent<Indicator>();
	comp->_indicatorImage = obj->AddComponent<HDData::ImageUI>();
	comp->_indicatorImage->GetTransform()->SetPosition(API::GetScreenWidth() / 2.0f, API::GetScreenHeight() / 2.0f, 0);
	comp->_indicatorImage->SetImage("DamegeIndicator.png");
	//_indicatorImage->ChangeScale(1.5f, 1.5f);
	float width = comp->_indicatorImage->GetImageWidth();
	float height = comp->_indicatorImage->GetImageHeight();
	//_indicatorImage->SetOrigin(width / 2.0f * 0.65f, height / 2.0f * 0.7f);
	comp->_indicatorImage->SetOrigin(width / 2.0f * 0.975f, height / 2.0f * 1.05f);
	//_indicatorImage->SetCenter(-100, -100);
	comp->_indicatorImage->SetCenter(0, 0);
	comp->_indicatorImage->GetGameObject()->SetSelfActive(false);
	return comp;
}

void Indicator::Update()
{
	// 각도 계산
	auto playerObj = IndicatorPool::Instance().player;
	Vector3 targetDir = _targetPos - playerObj->GetTransform()->GetPosition();
	Vector3 forward = IndicatorPool::Instance().player->GetTransform()->GetForward();
	float radian = CalculateDegreeBetweenTwoVectors(forward, targetDir);
	_indicatorImage->SetAngle(radian);
	_indicatorTimer.Update();
}

void Indicator::SetEffectOn(Vector3 targetPos)
{
	// 각도 계산
	_targetPos = targetPos;
	auto playerObj = IndicatorPool::Instance().player;
	Vector3 playerPos = playerObj->GetTransform()->GetPosition();
	Vector3 targetDir = _targetPos - playerPos;
	Vector3 forward = playerObj->GetTransform()->GetForward();
	float radian = CalculateDegreeBetweenTwoVectors(forward, targetDir);
	_indicatorImage->SetAngle(radian);
	_indicatorTimer.Start();
}
