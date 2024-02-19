#include "Sampler.hlsli"

cbuffer externalData : register(b0)
{
    float4x4 inverseView;
    float4x4 inverseProj;
    float4 kernel[64];
    float4 cameraPosition;
    float radius;
    float2 windowSize;
}

struct VertexToPixel
{
    float4 position     : SV_POSITION; // vertex position
    float2 uv           : TEXCOORD;
};

Texture2D DepthTex : register(t0);
Texture2D NormalTex : register(t1);
Texture2D Noise     : register(t2);

float3 CalculateWorldFromDepth(float depth, float2 texCoord)
{
    // clip space between [-1, 1]
    // flip y so that +ve y is upwards
    float2 clipXY = texCoord * 2.0 - 1.0;
    clipXY.y = -clipXY.y;

    // NOTE: depth is not linearized
    // Also in range [0, 1] due to DirectX Convention
    float4 clipSpace = float4(clipXY, depth, 1);
    float4 viewSpace = mul(inverseProj, clipSpace);

    // perspective divide
    viewSpace /= viewSpace.w;

    float4 worldSpace = mul(inverseView, viewSpace);
    return worldSpace.xyz;
}

float4 main(VertexToPixel input) : SV_TARGET
{
    float depth = DepthTex.Sample(PointSampler, input.uv).r;
    if (depth >= 1)
        return float4(1.0f, 1.0f, 1.0f, 1.0f);

    float3 posW = CalculateWorldFromDepth(depth, input.uv);
    float3 normal = NormalTex.Sample(PointSampler, input.uv).rgb;
    normal = normalize(normal * 2.0f - 1.0f);
    float centerDepth = distance(posW, cameraPosition.xyz);
    float2 noiseScale = windowSize / 4.0f;
    float3 randDir = Noise.Sample(PointSampler, input.uv * noiseScale).rgb;

    float3 tangent = normalize(randDir - normal * dot(randDir, normal));
    float3 bitangent = cross(normal, tangent);
    float3x3 tbn = transpose(float3x3(tangent, bitangent, normal));

    float occlusion = 0.0f;
    for (int i = 0; i < 64; ++i)
    {
        // find out a desired world position to sample
        float3 kernelPosW = mul(tbn, kernel[i].rgb);
        float3 samplePosW = posW + kernelPosW * radius;
        float sampleDepth = distance(samplePosW, cameraPosition.xyz);

        // project it to the clip space so we know where can sample from the depth buffer
        //float4x4 viewProj = mul(inverseProj, inverseView);
        float4x4 viewProj = mul(inverseView, inverseProj);
        float4 samplePosClip = mul(viewProj, float4(samplePosW, 1.0f));
        samplePosClip /= samplePosClip.w;

        // invert y and put to [0 - 1]
        float2 sampleUV = float2(samplePosClip.x, -samplePosClip.y) * 0.5f + 0.5f;

        // reject samples outside of the range
        if (sampleUV.x < 0 || sampleUV.x > 1 || sampleUV.y < 0 || sampleUV.y > 1)
        {
            occlusion += 0.0f;
            continue;
        }

        // sample our scene for actual depth
        float depthFromTex = DepthTex.Sample(PointSampler, sampleUV.xy).r;
        float3 scenePos = CalculateWorldFromDepth(depthFromTex, sampleUV.xy);
        float sceneDepth = distance(scenePos, cameraPosition.xyz);

        float depthDiff = abs(sceneDepth - centerDepth);
        float rangeCheck = smoothstep(0.0f, 1.0f, radius / depthDiff);
        occlusion += step(sceneDepth, sampleDepth) * rangeCheck;
    }
    occlusion /= 64.0f;
    float factor = 1 - occlusion;
    return float4(factor.rrr, 1.0f);
}