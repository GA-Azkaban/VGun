#include "Sampler.hlsli"
#define EV100 1.0
#define GAMMA 2.2

Texture2D src : register(t0);

struct VertexToPixel
{
	float4 position     : SV_POSITION; // vertex position
	float2 uv           : TEXCOORD;
};

float3 AcesHdrToLdr(float3 hdr)
{
	float3x3 m1 = float3x3(
		float3(0.59719f, 0.07600f, 0.02840f),
		float3(0.35458f, 0.90834f, 0.13383f),
		float3(0.04823f, 0.01566f, 0.83777f)
		);
	float3x3 m2 = float3x3(
		float3(1.60475f, -0.10208, -0.00327f),
		float3(-0.53108f, 1.10813, -0.07276f),
		float3(-0.07367f, -0.00605, 1.07602f)
		);

	float3 v = mul(hdr, m1);
	float3 a = v * (v + float3(0.0245786f, 0.0245786f, 0.0245786f)) - float3(0.000090537f, 0.000090537f, 0.000090537f);
	float3 b = v * (float3(0.983729f, 0.983729f, 0.983729f) * v + float3(0.4329510f, 0.4329510f, 0.4329510f)) + float3(0.238081f, 0.238081f, 0.238081f);
	float3 ldr = saturate(mul(a / b, m2));

	return ldr;
}


float3 AdjustExposure(float3 color)
{
	float LMax = (78.0f / (0.65f * 100.0f)) * pow(2.0f, EV100);
	return color * (1.0f / LMax);
}

float3 CorrectGamma(float3 color, float gamma)
{
	return pow(color, 1.0f / gamma);
}

void WriteLuma(inout float4 color)
{
	float luma = dot(color.rgb, float3(0.2126, 0.7152, 0.0722));
	color.a = luma;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	//float3 hdrColor = src.Sample(PointSampler, input.uv).rgb;
	//float3 ldrColor = AdjustExposure(hdrColor);
	////float3 ldrColor = hdrColor;
	//ldrColor = AcesHdrToLdr(ldrColor);
	//float4 outColor = float4(CorrectGamma(ldrColor, GAMMA), 1.0);
	////float4 outColor = float4(ldrColor, 1.0f);
	//WriteLuma(outColor);

	//return outColor;
	
	float3 color = src.Sample(PointSampler, input.uv).rgb;
	//color *= 0.6;
	//float a = 2.51f;
	//float b = 0.03f;
	//float c = 2.43f;
	//float d = 0.59f;
	//float e = 0.14f;

	//float3 result = clamp((color * (a * color + b)) / (color * (c * color + d) + e), 0.0f, 1.0f);
	//result = pow(result, 1.0f / GAMMA);
	//return float4(result, 1.0f);
	return float4(color, 1.0f);
}