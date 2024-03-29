#include "HDTransform.h"

namespace HDCommon
{
	Transform::Transform()
		:_position(0.0f, 0.0f, 0.0f), _rotation(0.0f, 0.0f, 0.0f, 1.0f), _scale(Vector3::One) 
	{
		
	}
}
