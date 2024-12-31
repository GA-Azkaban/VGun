#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>
using namespace DirectX;

struct MinMaxElements
{
	float minX = FLT_MAX;
	float maxX = -FLT_MAX;
	float minY = FLT_MAX;
	float maxY = -FLT_MAX;
	float minZ = FLT_MAX;
	float maxZ = -FLT_MAX;
};

class BoundingVolumeHelper
{
public:
	static DirectX::BoundingBox BoundingBoxFromRange(const MinMaxElements& elements)
	{
		XMFLOAT3 lower_left(elements.minX, elements.minY, elements.minZ);
		XMFLOAT3 upper_right(elements.maxX, elements.maxY, elements.maxZ);
		XMFLOAT3 center((lower_left.x + upper_right.x) * 0.5f, (lower_left.y + upper_right.y) * 0.5f, (lower_left.z + upper_right.z) * 0.5f);
		XMFLOAT3 extents(upper_right.x - center.x, upper_right.y - center.y, upper_right.z - center.z);

		return BoundingBox(center, extents);
	}
};
