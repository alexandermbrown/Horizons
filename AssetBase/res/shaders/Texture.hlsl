
struct VS_IN
{
	float2 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

cbuffer ViewProjectionMatrix : register(b0)
{
	float4x4 u_ViewProj;
};

cbuffer TransformMatrix : register(b1)
{
	float4x4 u_Transform;
};

PS_IN vs_main(VS_IN input)
{
	PS_IN output;

	output.position = mul(u_ViewProj, mul(u_Transform, float4(input.position, 0.0, 1.0)));
	output.texcoord = input.texcoord;

	return output;
}

Texture2D u_Texture : register( t0 );
SamplerState u_Sampler : register( s0 );

float4 ps_main(PS_IN input) : SV_TARGET
{
	return u_Texture.Sample(u_Sampler, input.texcoord);
}