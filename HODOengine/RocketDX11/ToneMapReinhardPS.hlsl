#include "Sampler.hlsli"

Texture2D src : register(t0);

struct VertexToPixel
{
    float4 position     : SV_POSITION; // vertex position
    float2 uv           : TEXCOORD;
};

float CalcLuminance(float3 color)
{
	return dot(color.xyz, float3(0.299f, 0.587f, 0.114f));
}

float4 main(VertexToPixel input) : SV_TARGET
{
    float3 color = src.Sample(PointSampler, input.uv).rgb;
    float luminance = CalcLuminance(color);
    float reinhard = luminance / (luminance + 1);
    return float4(color * (reinhard / luminance), 1.0f);
    //return float4(color, 1.0f);
}