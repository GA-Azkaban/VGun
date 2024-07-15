#pragma once
#include "IRenderable.h"

namespace HDEngine
{
	class ISketchable : public IRenderable
	{
	public:
		virtual ~ISketchable() {};
		virtual void SetScreenSpace() = 0;
		virtual void SetWorldSpace() = 0;
		virtual void SetSortOrder(float order) = 0;

		virtual float GetWidth() = 0;
		virtual float GetHeight() = 0;
	
		virtual bool SetFadeMode(bool SetFadem,float time) = 0;
		virtual void FadeIn(float time) = 0;
		virtual void FadeOut(float time) = 0;
		virtual bool GetFadeMode() = 0;
		virtual bool GetComplete() = 0;
	};
}
