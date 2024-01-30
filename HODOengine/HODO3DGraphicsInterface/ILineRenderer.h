#pragma once
#include "MathHeader.h"

namespace HDEngine
{
	class ILineRenderer
	{
	public:
		virtual void DrawLine(Vector3 startPos, Vector3 endPos, Color color) = 0;
		virtual void DrawLine(Vector3 startPos, Vector3 direction, float length, Color color) = 0;
	};
}