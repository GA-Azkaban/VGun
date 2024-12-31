#include "Indicator.h"
#include "IndicatorPool.h"
#include <cmath>
#include <numbers>

Indicator::Indicator()
{
	_indicatorTimer.isRepeat = false;
	_indicatorTimer.duration = 1.0f;
	_indicatorTimer.onUpdate = [&](float progress) {
		auto playerObj = IndicatorPool::Instance().player;
		Vector3 forward = playerObj->GetTransform()->GetForward();
		Vector3 targetDir = _targetPos - playerObj->GetTransform()->GetPosition();
		// Calculate degree between two vectors
		targetDir.Normalize();
		float scalar = forward.x * targetDir.x + forward.z * targetDir.z;
		float length1 = forward.Length();
		float length2 = targetDir.Length();
		float radian = acosf(scalar / (length1 * length2));
		// if cross product y value is negative, reverse the angle
		Vector3 crossVec = forward.Cross(targetDir);
		if (crossVec.y < 0)
		{
			float degree = radian * 180.0f / std::numbers::pi;
			radian = (360.0f - degree) * std::numbers::pi / 180.0f;
		}
		_indicatorImage->SetAngle(radian); };
	_indicatorTimer.onExpiration = [&]() { IndicatorPool::Instance().RetrieveIndicator(this); };
}

Indicator* Indicator::CreateIndicator()
{
	auto obj = API::CreateObject(API::GetCurrentScene());
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
	comp->GetGameObject()->SetSelfActive(false);
	return comp;
}

void Indicator::Update()
{
	_indicatorTimer.Update();
}

void Indicator::SetEffectOn(Vector3 targetPos)
{
	_targetPos = targetPos;
	_indicatorTimer.Start();
}
