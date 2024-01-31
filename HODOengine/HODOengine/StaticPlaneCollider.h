#pragma once
#include "StaticCollider.h"


namespace HDData
{
	class HODO_API StaticPlaneCollider : public StaticCollider
	{
	public:
		StaticPlaneCollider();

	public:

	public:
		// plane angle
		Vector3 GetNormalVector() const;
		// how far from bottom
		float GetDistance() const;

	private:
		Vector3 _normalVector;
		float _distance;
	};
}
