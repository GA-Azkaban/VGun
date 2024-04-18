static const float2 arrBasePos[4] =
{
    float2(-1.0, 1.0),
    float2(1.0, 1.0),
    float2(-1.0, -1.0),
    float2(1.0, -1.0),
};
static const float2 arrUV[4] =
{
    float2(0.0, 0.0),
    float2(1.0, 0.0),
    float2(0.0, 1.0),
    float2(1.0, 1.0),
};

struct VertexToPixel
{
    float4 position     : SV_POSITION; // vertex position
    float2 uv           : TEXCOORD;
};

VertexToPixel main(uint VertexID : SV_VertexID)
{
    VertexToPixel output;

    output.position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
    output.uv = arrUV[VertexID].xy;

    return output;
}