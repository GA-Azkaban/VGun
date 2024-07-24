#pragma once
#include "MathHeader.h"

namespace HDEngine
{
	class ILight
	{
	public:
		virtual ~ILight() {}
		virtual void SetPosition(const Vector4& position) = 0;
		virtual void SetDirection(const Vector4& direction) = 0;
		virtual void SetColor(const Vector4& color) = 0;
		virtual void SetLightType(unsigned int type) = 0;		
		virtual void SetActive(bool isActive) = 0;
	};
}
