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

	// linear interpolation
	static DirectX::XMFLOAT3 Mix(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2, float a)
	{
		DirectX::XMFLOAT3 ret;
		DirectX::XMVECTOR xmVec1 = DirectX::XMLoadFloat3(&v1);
		DirectX::XMVECTOR xmVec2 = DirectX::XMLoadFloat3(&v2);
		DirectX::XMVECTOR mix = (xmVec1 * (1.0f - a)) + (xmVec2 * a);
		XMStoreFloat3(&ret, mix);		
		return ret;
	}

	static DirectX::XMFLOAT4 Mix(const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2, float a)
	{
		DirectX::XMFLOAT4 ret;
		DirectX::XMVECTOR xmVec1 = DirectX::XMLoadFloat4(&v1);
		DirectX::XMVECTOR xmVec2 = DirectX::XMLoadFloat4(&v2);
		DirectX::XMVECTOR mix = (xmVec1 * (1.0f - a)) + (xmVec2 * a);
		XMStoreFloat4(&ret, mix);
		return ret;
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
