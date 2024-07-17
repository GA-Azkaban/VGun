#include "UIEffect.h"
#include <algorithm>


UIEffect::UIEffect(Vector2 destScale, float time, bool isFade, float power)
	:_isPlay(false), _time(time), _start(destScale), _isShake(false), _isLeft(true), _isFade(isFade),
	_power(power)
{

}

UIEffect::~UIEffect()
{

}

void UIEffect::Start()
{
	_ui = GetGameObject()->GetComponent<HDData::ImageUI>();
	_originPos = { GetGameObject()->GetTransform()->GetPosition().x, GetGameObject()->GetTransform()->GetPosition().y };
	GetGameObject()->SetSelfActive(false);
	_scaleTimer = new Timer;
	_scaleTimer->duration = _time;
	_scaleTimer->onExpiration = [=]()
		{
			_scaleTimer->Stop();
			_isPlay = false;
			_isShake = true;
			_shakeTimer->Start();
		};

	_shakeTimer = new Timer;
	_shakeTimer->duration = 1;
	_shakeTimer->onExpiration = [=]()
		{
			EndEffect();
		};

	auto imgscale = _ui->GetImageScale();
	_originScale = { imgscale.x, imgscale.y };
}

void UIEffect::Update()
{
	if (_isPlay)
	{
		ScaleUpdate();
	}

	if (_isShake)
	{
		Shake();
	}
}

void UIEffect::Play()
{
	GetGameObject()->SetSelfActive(true);
	_scaleTimer->Start();
	_ui->ChangeScale(_start.x, _start.y);
	_isPlay = true;
}

void UIEffect::ScaleUpdate()
{
	_scaleTimer->Update();
	auto elapsed = _scaleTimer->GetElapsedTime() / _scaleTimer->duration;

	// 시작 스케일과 끝 스케일 정의
	float startScaleX = _start.x;
	float startScaleY = _start.y;
	float endScaleX = 1.0f;
	float endScaleY = 1.0f;

	// 스케일 계산
	float currentScaleX = (startScaleX - (startScaleX - endScaleX) * elapsed) * _originScale.x;
	float currentScaleY = (startScaleY - (startScaleY - endScaleY) * elapsed) * _originScale.y;

	// UI 스케일 변경
	_ui->ChangeScale(currentScaleX, currentScaleY);
}

void UIEffect::Shake()
{
	_shakeTimer->Update();

	float newPower = _power *  (1 - (_shakeTimer->GetElapsedTime() / _shakeTimer->duration));

	if (_shakeTimer->duration - _shakeTimer->GetElapsedTime() < 0.04)
	{
		_ui->GetTransform()->SetPosition(_originPos.x, _originPos.y, 0);
		return;
	}

	if (_isLeft)
	{
		_ui->GetTransform()->SetPosition(_originPos.x + newPower, _originPos.y + newPower, 0);
	}
	else
	{
		_ui->GetTransform()->SetPosition(_originPos.x - newPower, _originPos.y - newPower, 0);
	}

	_isLeft = !_isLeft;
}

void UIEffect::EndEffect()
{
	if(_isFade) _ui->FadeOut(2.f);
	_shakeTimer->Stop();
	_isShake = false;
}
