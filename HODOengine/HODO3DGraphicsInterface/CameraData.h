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

		float nearZ;					// frustum�� ����� �������� �Ÿ�
		float farZ;					// frustum�� �� �������� �Ÿ�
		float aspect;					// ���� / ���� ����
		float fovY;					// fov������ 60�й����� ��������
		float nearWindowHeight;		// frustum�� ����� ����� ����
		float farWindowHeight;			// frustum�� �� ����� ����
	};
}