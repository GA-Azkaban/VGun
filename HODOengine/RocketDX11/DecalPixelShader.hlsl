#include "ConstantBuffer.hlsli"
#include "Sampler.hlsli"
#include "Common.hlsli"

Texture2D Albedo : register(t0);
Texture2D DepthTex : register(t1);

struct VertexToPixel
{
    float4 position : SV_POSITION;
};

float4 main(VertexToPixel input) : SV_Target
{
    float2 screenPosition = input.position.xy / input.position.w;
    float2 depthCoords = screenPosition * float2(0.5f, -0.5f) + 0.5f;
    float depth = DepthTex.Sample(PointClampSampler, depthCoords).r;
    
    float4 viewSpacePosition = float4(CalculateViewSpaceFromDepth(depth, depthCoords), 1.0f);
    float4 worldSpacePosition = mul(viewSpacePosition, inverseView);
    float4 localSpacePosition = mul(worldSpacePosition, inverseWorld);
    localSpacePosition.xyz /= localSpacePosition.w;
    
    clip(0.5f - abs(localSpacePosition.xyz));
    
    float4 albedo = Albedo.SampleLevel(LinearWrapSampler, localSpacePosition.xz + 0.5f, 0);
    //if (albedo.a < 0.1)
    //    discard;
    
    return albedo * albedoColor;
    //return float4(1.0f, 0.0f, 0.0f, 1.0f);
}