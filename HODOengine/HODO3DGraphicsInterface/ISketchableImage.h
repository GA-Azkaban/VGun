#pragma once
#include "ISketchable.h"

namespace HDEngine
{
	class ISketchableImage : public ISketchable
	{
	public:
		virtual void SetImage(const char* fileName) = 0;
	};
}