
struct VertexIn
{
    float3 PosL  : POSITION;
    float4 Color : COLOR;
};

struct VertexOut
{
    float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

struct PixelOut
{
    float4 Color    : SV_Target0;  
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    vout.PosH = float4(vin.PosL, 1.0f);

    // Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;

    return vout;
}

PixelOut PS(VertexOut pin)
{
    PixelOut output;
    output.Color = pin.Color;

    return output;
}


technique11 DrawUITextTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
