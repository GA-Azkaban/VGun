#include "UIEffect.h"
#include <algorithm>


UIEffect::UIEffect(Vector2 destScale, float time)
	:_isPlay(false), _time(time), _start(destScale), _isShake(false), _isLeft(true)
{

}

UIEffect::~UIEffect()
{

}

void UIEffect::Start()
{
	_ui = GetGameObject()->GetComponent<HDData::ImageUI>();
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
	//_ui->SetFadeMode(false, 1);
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
	float currentScaleX = startScaleX - (startScaleX - endScaleX) * elapsed;
	float currentScaleY = startScaleY - (startScaleY - endScaleY) * elapsed;

	// UI 스케일 변경
	_ui->ChangeScale(currentScaleX, currentScaleY);
}

void UIEffect::Shake()
{
	_shakeTimer->Update();

	auto origin = _ui->GetGameObject()->GetTransform()->GetPosition();

	if (_isLeft)
	{
		auto x = origin.x - 5;
		auto y = origin.y - 5;
		_ui->GetTransform()->SetPosition(x, y, 0);
	}
	else
	{
		auto x = origin.x + 5;
		auto y = origin.y + 5;
		_ui->GetTransform()->SetPosition(x, y, 0);
	}

	_isLeft = !_isLeft;
}

void UIEffect::EndEffect()
{
	//_ui->FadeOut(2.f);
	_shakeTimer->Stop();
	_isShake = false;
}
