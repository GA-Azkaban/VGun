#include "ConstantBuffer.hlsli"

struct VertexShaderInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
};

struct VertexToPixel
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

VertexToPixel main(VertexShaderInput input, uint instanceID : SV_InstanceID)
{
    VertexToPixel output;

    float4 pos = mul(float4(input.position, 1.0f), particleTransforms[instanceID]);
    output.position = mul(pos, viewProjection);
    output.uv = input.uv;
    output.color = particleColors[instanceID];

    return output;
}