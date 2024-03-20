#include "Sampler.hlsli"
#include "ConstantBuffer.hlsli"

Texture2D StencilColorTex	: register(t0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

static float xFilter[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
static float yFilter[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
//static float xFilter[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
//static float yFilter[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

static float3 grayScale = float3(0.299, 0.587, 0.114);

float4 main(VertexToPixel input) : SV_TARGET
{
	float2 tx = float2(outlineThickness / screenWidth, 0.0);
	float2 ty = float2(0.0, outlineThickness / screenHeight);

	float grid[9];
	grid[0] = length(dot(StencilColorTex.Sample(PointClampSampler, input.uv - tx + ty).rgb, grayScale));
	grid[1] = length(dot(StencilColorTex.Sample(PointClampSampler, input.uv + ty).rgb, grayScale));
	grid[2] = length(dot(StencilColorTex.Sample(PointClampSampler, input.uv + tx + ty).rgb, grayScale));
	grid[3] = length(dot(StencilColorTex.Sample(PointClampSampler, input.uv - tx).rgb, grayScale));
	grid[4] = length(dot(StencilColorTex.Sample(PointClampSampler, input.uv).rgb, grayScale));
	grid[5] = length(dot(StencilColorTex.Sample(PointClampSampler, input.uv + tx).rgb, grayScale));
	grid[6] = length(dot(StencilColorTex.Sample(PointClampSampler, input.uv - tx - ty).rgb, grayScale));
	grid[7] = length(dot(StencilColorTex.Sample(PointClampSampler, input.uv - ty).rgb, grayScale));
	grid[8] = length(dot(StencilColorTex.Sample(PointClampSampler, input.uv + tx - ty).rgb, grayScale));

	float sx = 0.0f;
	float sy = 0.0f;
	for (int i = 0; i < 9; ++i)
	{
		//sx += grid[i] * xFilter[i];
		//sy += grid[i] * yFilter[i];
		sx += grid[i];
		sy += grid[i];
	}

	float dist = sqrt(sx * sx + sy * sy);
	float edge = dist > outlineThreshHold ? 1 : 0;

	float4 result = outlineColor;
	result.a = result.a * edge;

	//return result;
	return float4(dist, dist, dist, 1.0f);
}