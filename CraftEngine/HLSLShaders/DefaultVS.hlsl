// transform constant buffer.
cbuffer TransformBuffer : register(b0)
{
    //world matrix
    row_major matrix world;
};


float4 main( float3 pos : POSITION ) : SV_POSITION
{
    float4 worldPosition = mul(float4(pos, 1), world);
    return worldPosition;
}