
struct VERTEX_IN
{
	float2 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct INSTANCE_IN
{
	column_major float4x4 transform : I_TRANSFORM;
	float4 atlasBounds : I_ATLASBOUNDS;
	float4 color : COLOR;
};

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
};

cbuffer ViewProjectionMatrix : register(b0)
{
	column_major float4x4 u_ViewProj;
};

PS_IN vs_main(VERTEX_IN vertex, INSTANCE_IN instance)
{
	PS_IN output;

	output.position = mul(u_ViewProj, mul(instance.transform, float4(vertex.position, 0.0, 1.0)));

	output.texcoord = float2(
		instance.atlasBounds.x + instance.atlasBounds.z * vertex.texcoord.x,
		instance.atlasBounds.y + instance.atlasBounds.w * vertex.texcoord.y);
	output.color = instance.color;

	return output;
}

Texture2D u_Texture : register( t0 );
SamplerState u_Sampler : register( s0 );

float4 ps_main(PS_IN input) : SV_TARGET
{
	return input.color * u_Texture.Sample(u_Sampler, input.texcoord);
}