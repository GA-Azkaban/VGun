#pragma once
#include "..\\HODOmath\\HODOmath.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\HODOmath.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\HODOmath.lib")
#endif // _DEBUG

namespace HDEngine
{
	struct CameraData
	{
		HDMath::HDFLOAT3 position;
		HDMath::HDQuaternion rotation;

		float nearZ;					// frustum의 가까운 평면까지의 거리
		float farZ;					// frustum의 먼 평면까지의 거리
		float aspect;					// 가로 / 세로 비율
		float fovY;					// fov각도를 60분법으로 갖고있음
		float nearWindowHeight;		// frustum의 가까운 평면의 높이
		float farWindowHeight;			// frustum의 먼 평면의 높이
	};
}