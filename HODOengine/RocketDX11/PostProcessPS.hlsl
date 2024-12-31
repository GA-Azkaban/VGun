#include "ConstantBuffer.hlsli"
#include "Sampler.hlsli"

static const float M_PI_DIV_4 = 0.7853981635f;

Texture2D<float4> vignetteSrc : register(t0);

float3 ApplyVignette(float3 color, float2 uv)
{
    if (vignetteEnabled)
    {
        const float2 uvCenter = float2(0.5f, 0.5f);
        float2 uvFromCenter = abs(uv - uvCenter) / float2(uvCenter);

        float2 vignetteMask = cos(uvFromCenter * vignetteIntensity * M_PI_DIV_4);
        vignetteMask = vignetteMask * vignetteMask;
        vignetteMask = vignetteMask * vignetteMask;
        color *= clamp(vignetteMask.x * vignetteMask.y, 0, 1);
    }
    return color;
}

struct VertexToPixel
{
    float4 position : SV_POSITION; // vertex position
    float2 uv : TEXCOORD;
};

float4 main(VertexToPixel input) : SV_TARGET
{
    float2 uv = input.uv;
    //float3 color = vignetteSrc.SampleLevel(LinearBorderSampler, uv, 0).rgb;
    float3 color = vignetteSrc.Sample(LinearWrapSampler, uv).rgb;
    color = ApplyVignette(color, uv);
    return float4(color, 1.0f) * albedoColor;
}