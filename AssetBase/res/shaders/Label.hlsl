
struct VS_IN
{
	float3 position : POSITION;
};

struct PS_IN
{
	float4 position : SV_POSITION;
};

cbuffer VS_MATRICES : register(b0)
{
	float4x4 u_ViewProj;
	float4x4 u_Transform;
};

PS_IN vs_main(VS_IN input)
{
	PS_IN output;
	output.position = mul(u_ViewProj, mul(u_Transform, float4(input.position, 1.0)));
	return output;
}

float4 ps_main(PS_IN input) : SV_TARGET
{
	return float4(0.4, 0.8, 0.6, 1.0);
}