#include "UIEffect.h"
#include <algorithm>


UIEffect::UIEffect(Vector2 destScale, float time)
	:_isPlay(false), _time(time), _start(destScale), _isShake(false)
{

}

UIEffect::~UIEffect()
{

}

void UIEffect::Start()
{
	//GetGameObject()->SetSelfActive(false);

	_ui = GetGameObject()->GetComponent<HDData::Button>()->GetButtonComp();

	_timer = new Timer;
	_timer->duration = _time;
	_timer->onExpiration = [=]()
		{
			_timer->Stop();
			_isPlay = false;
			//_timer->duration = 3;
			//_timer->onExpiration = [=]() {
			//	EndEffect();
			//	};
			//Shake();
			//_timer->Start();
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
		//Shake();
	}
}

void UIEffect::Play()
{
	//GetGameObject()->SetSelfActive(true);
	_timer->Start();
	_ui->ChangeScale(_start.x, _start.y);
	_isPlay = true;
}

void UIEffect::ScaleUpdate()
{
	_timer->Update();
	auto elapsed = _timer->GetElapsedTime() / _timer->duration;

	// 시작 스케일과 끝 스케일 정의
	float startScaleX = _start.x;
	float startScaleY = _start.y;
	float endScaleX = 1.0f;
	float endScaleY = 1.0f; 

	// 선형 보간을 통해 스케일 계산
	float currentScaleX = startScaleX - (startScaleX - endScaleX) * elapsed;
	float currentScaleY = startScaleY - (startScaleY - endScaleY) * elapsed;

	// UI 스케일 변경
	_ui->ChangeScale(1.2, 1.2);
}

void UIEffect::Shake()
{
	_timer->Update();
	auto elapse = _timer->GetElapsedTime() / _timer->duration;
	float xOffset = static_cast<float>(std::sin(elapse * 1) * 1);
	float yOffset = static_cast<float>(std::cos(elapse * 1) * 1);

	auto origin = _ui->GetGameObject()->GetTransform()->GetPosition();
	_ui->GetGameObject()->GetTransform()->SetPosition(origin.x + xOffset, origin.y + yOffset, 0);
}

void UIEffect::EndEffect()
{
	//_ui->FadeOut(2.f);
	_isShake = false;
}
