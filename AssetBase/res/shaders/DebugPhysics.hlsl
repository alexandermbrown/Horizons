
struct VS_IN
{
	float2 position : POSITION;
	float4 color : COLOR;
};

struct PS_IN
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer ViewProjectionMatrix : register(b0)
{
	float4x4 u_ViewProj;
};

PS_IN vs_main(VS_IN input)
{
	PS_IN output;

	output.position = mul(u_ViewProj, float4(input.position, 0.0, 1.0));
	output.color = input.color;

	return output;
}

float4 ps_main(PS_IN input) : SV_TARGET
{
	return input.color;
}