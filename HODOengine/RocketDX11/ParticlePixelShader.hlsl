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
    float4 textureColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (useAlbedo)
    {
        textureColor = Albedo.Sample(LinearWrapSampler, input.uv);
    }
    if (textureColor.a < 0.15f)
    {
        discard;
    }
    return textureColor * input.color * albedoColor;
}