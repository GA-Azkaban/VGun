#include "Sampler.hlsli"
#define GAMMA 2.2

Texture2D src : register(t0);

struct VertexToPixel
{
    float4 position     : SV_POSITION; // vertex position
    float2 uv           : TEXCOORD;
};

float CalcLuminance(float3 color)
{
	return dot(color.xyz, float3(0.2126f, 0.7152f, 0.0722f));
}

float4 main(VertexToPixel input) : SV_TARGET
{
    float3 color = src.Sample(PointClampSampler, input.uv).rgb;
    float luminance = CalcLuminance(color);
    float reinhard = luminance / (luminance + 1);
    if (luminance > 1e-6)
        color *= reinhard / luminance;
    color = pow(color, 1.0f / GAMMA);
    return float4(color, 1.0f);
}