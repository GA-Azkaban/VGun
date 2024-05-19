#include "Sampler.hlsli"
#include "ConstantBuffer.hlsli"

Texture2D Albedo : register(t0);

struct VertexToPixel
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

float4 main(VertexToPixel input) : SV_TARGET
{
    float3 albedo = Albedo.Sample(LinearWrapSampler, input.uv).rgb;
    return float4(albedo, 1.0f) * input.color * albedoColor;
}