#pragma once
#include "../HODOmath/HODOmath.h"

namespace HDEngine
{
	class ILineRenderer
	{
	public:
		virtual void DrawLine(HDMath::HDFLOAT3 startPos, HDMath::HDFLOAT3 endPos, HDMath::HDFLOAT4 color) = 0;
		virtual void DrawLine(HDMath::HDFLOAT3 startPos, HDMath::HDFLOAT3 direction, float length, HDMath::HDFLOAT4 color) = 0;
	};
}