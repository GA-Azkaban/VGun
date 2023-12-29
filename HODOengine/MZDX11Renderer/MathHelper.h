#pragma once
#include <DirectXMath.h>
using namespace DirectX;

/// <summary>
/// 상수나 수식 등을 정의한 helper math 클래스.
/// 2023.07.03 MJKIM
/// </summary>

class MathHelper
{
public:
	template <typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template <typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template <typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a) * t;
	}

	template <typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float AngleFromXY(float x, float y);

	static XMMATRIX InverseTranspose(CXMMATRIX M)
	{
		XMMATRIX A = M;
		//A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR det = XMMatrixDeterminant(A);
		A = XMMatrixInverse(&det, A);
		return XMMatrixTranspose(A);
	}

	static const float Infinity;
	static const float Pi;
};
