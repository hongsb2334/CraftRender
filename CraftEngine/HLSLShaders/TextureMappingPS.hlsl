// Vertex Shader Output
struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

//Texture
Texture2D diffuseTexture : register(t0);

//Sampler
SamplerState diffuseSampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 diffuseColor = diffuseTexture.Sample(diffuseSampler, input.uv);
	return diffuseColor;
    
}