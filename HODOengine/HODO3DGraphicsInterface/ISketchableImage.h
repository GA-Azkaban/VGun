#pragma once
#include "ISketchable.h"

namespace HDEngine
{
	class ISketchableImage : public ISketchable
	{
	public:
		virtual void SetImage(const char* fileName) = 0;
		virtual void SetActive(bool active) = 0;
		virtual void SetScreenSpace() = 0;
		virtual void SetWorldSpace() = 0;
	};
}