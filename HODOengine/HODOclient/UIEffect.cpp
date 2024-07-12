#include "UIEffect.h"

UIEffect::UIEffect(Vector3& start, Vector3 end, HDData::eEasing type)
	: _start(start), _end(end), type(type)
{
	_tween = API::CreateTween();
	_originposCopied = start;
	_tween->OnComplete([this]()
		{
			SetOrigin();
		});
}

UIEffect::~UIEffect()
{
	delete _tween;
}

void UIEffect::Play()
{
	_tween->GetData(&_start).DoMove(_end, 1).SetEase(type);
}

void UIEffect::SetOrigin()
{
	_start = _originposCopied;
}
