//Vertex Shader Input
struct VSInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

// Vertex Shader Output
struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

// transform constant buffer.
cbuffer TransformBuffer : register(b0)
{
    //world matrix
    row_major matrix world;
};

//camera constanct buffer
cbuffer CameraBuffer : register(b1)
{
    row_major matrix view;
    row_major matrix projection;
}

//sv = systemvalue

VSOutput main( VSInput input )
{
    VSOutput output;
    
    //WVP(WORLD-VIEW-PROJECTION) Transformation
    output.position = mul(float4(input.position, 1), world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.uv = input.uv;
    
    return output;
}