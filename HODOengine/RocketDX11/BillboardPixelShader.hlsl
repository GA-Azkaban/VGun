#include "Sampler.hlsli"
#include "ConstantBuffer.hlsli"

Texture2D Albedo : register(t0);

struct VertexToPixel
{
    float4 position : SV_POSITION; // vertex position
    float2 uv : TEXCOORD;
};

float4 main(VertexToPixel input) : SV_TARGET
{
    float3 albedo = Albedo.Sample(LinearWrapSampler, input.uv).rgb;
    //float3 albedo = Albedo.Sample(PointClampSampler, input.uv).rgb;
    return float4(albedo, 1.0f) * albedoColor;
}