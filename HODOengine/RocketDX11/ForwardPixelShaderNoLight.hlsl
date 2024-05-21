#include "Sampler.hlsli"
#include "ConstantBuffer.hlsli"

Texture2D Albedo : register(t0);

struct VertexToPixel
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 worldPos : POSITION;
    float2 uv : TEXCOORD;
};

float4 main(VertexToPixel input) : SV_Target
{
	float4 textureColor = Albedo.Sample(LinearWrapSampler, input.uv);   
    return textureColor * albedoColor;
}