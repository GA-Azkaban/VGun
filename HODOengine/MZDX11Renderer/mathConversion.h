#pragma once
#include <DirectXMath.h>
using namespace DirectX;

namespace mathConversion
{
	inline XMVECTOR ToXMVector(const double double4[4])
	{
		XMVECTOR ret;
		ret.m128_f32[0] =static_cast<float>(double4[0]);
		ret.m128_f32[1] =static_cast<float>(double4[1]);
		ret.m128_f32[2] =static_cast<float>(double4[2]);
		ret.m128_f32[3] =static_cast<float>(double4[3]);
		return ret;
	}
	inline XMMATRIX ToXMMatrix(const double double44[4][4])
	{
		XMMATRIX ret;

		ret.r[0].m128_f32[0] = double44[0][0];
		ret.r[0].m128_f32[1] = double44[0][1];
		ret.r[0].m128_f32[2] = double44[0][2];
		ret.r[0].m128_f32[3] = double44[0][3];

		ret.r[1].m128_f32[0] = double44[1][0];
		ret.r[1].m128_f32[1] = double44[1][1];
		ret.r[1].m128_f32[2] = double44[1][2];
		ret.r[1].m128_f32[3] = double44[1][3];

		ret.r[2].m128_f32[0] = double44[2][0];
		ret.r[2].m128_f32[1] = double44[2][1];
		ret.r[2].m128_f32[2] = double44[2][2];
		ret.r[2].m128_f32[3] = double44[2][3];

		ret.r[3].m128_f32[0] = double44[3][0];
		ret.r[3].m128_f32[1] = double44[3][1];
		ret.r[3].m128_f32[2] = double44[3][2];
		ret.r[3].m128_f32[3] = double44[3][3];

		return ret;
	}
	inline XMMATRIX ToXMMatrix(const double* double16)
	{
		XMMATRIX ret;

		ret.r[0].m128_f32[0] = double16[0];
		ret.r[0].m128_f32[1] = double16[1];
		ret.r[0].m128_f32[2] = double16[2];
		ret.r[0].m128_f32[3] = double16[3];

		ret.r[1].m128_f32[0] = double16[4];
		ret.r[1].m128_f32[1] = double16[5];
		ret.r[1].m128_f32[2] = double16[6];
		ret.r[1].m128_f32[3] = double16[7];

		ret.r[2].m128_f32[0] = double16[8];
		ret.r[2].m128_f32[1] = double16[9];
		ret.r[2].m128_f32[2] = double16[10];
		ret.r[2].m128_f32[3] = double16[11];

		ret.r[3].m128_f32[0] = double16[12];
		ret.r[3].m128_f32[1] = double16[13];
		ret.r[3].m128_f32[2] = double16[14];
		ret.r[3].m128_f32[3] = double16[15];

		return ret;
	}
}
