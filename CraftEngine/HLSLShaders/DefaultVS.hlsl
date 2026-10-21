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
}

//sv = systemvalue

float4 main( float3 pos : POSITION ) : SV_POSITION
{
    float4 worldPosition = mul(float4(pos, 1), world);
    float4 viewPosition = mul(worldPosition, view);
    
    
    return viewPosition;
}