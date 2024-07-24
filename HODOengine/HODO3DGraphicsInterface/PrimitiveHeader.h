#pragma once
#include "MathHeader.h"

namespace HDEngine
{
	struct PrimitiveBase
	{
		Matrix worldTM;
		bool isWire;
		Color color;
	};

	struct CubePrimitive : public PrimitiveBase
	{
		Vector3 widthHeightDepth;
	};

	struct SpherePrimitive : public PrimitiveBase
	{
		float diameter;
	};

	struct CylinderPrimitive : public PrimitiveBase
	{
		float height;
		float diameter;
	};

	struct CapsulePrimitive : public PrimitiveBase
	{
		float height;
		float diameter;
	};
}
