#pragma once
#include "MathHeader.h"

namespace HDCommon
{
	class Transform
	{
	public:
		Transform();
	//private:
		Vector3 _position;	// Local position of gameobject
		Quaternion _rotation;	// Local rotation of gameobject
		Vector3 _scale;	// Local scale of gameobject
	};
}
