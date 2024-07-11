#pragma once
#include "../HODOengine/HODO_API.h"

class UIEffect
{
public:
	UIEffect(Vector3& start, Vector3 end, HDData::eEasing type);
	~UIEffect();
	void Play();
	void SetOrigin();

private:
	bool _isStart;
	HDData::Tween* _tween;

	Vector3 _originposCopied;

	Vector3& _start;
	Vector3 _end;
	HDData::eEasing type;
};
