
struct VS_IN_STATIC
{
	float2 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct VS_IN_DYNAMIC
{
	float3 alphavalues : ALPHAVALUES;
};

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 alphavalues : ALPHAVALUES;
};

cbuffer ViewProjectionMatrix : register(b0)
{
	float4x4 u_ViewProj;
};

cbuffer TransformMatrix : register(b1)
{
	float4x4 u_Transform;
};

PS_IN vs_main(VS_IN_STATIC s_input, VS_IN_DYNAMIC d_input)
{
	PS_IN output;

	output.position = mul(u_ViewProj, mul(u_Transform, float4(s_input.position, 0.0, 1.0)));
	output.texcoord = s_input.texcoord;
	output.alphavalues = d_input.alphavalues;

	return output;
}

cbuffer AtlasBounds : register(b3)
{
	float4 u_AtlasBounds0;
	float4 u_AtlasBounds1;
	float4 u_AtlasBounds2;
	float4 u_AtlasBounds3;
};

Texture2D u_Texture : register( t0 );
SamplerState u_Sampler : register( s0 );

float4 ps_main(PS_IN input) : SV_TARGET
{
	float3 terraincolor = u_Texture.Sample(u_Sampler, float2(u_AtlasBounds0.x + u_AtlasBounds0.z * input.texcoord.x,
	u_AtlasBounds0.y + u_AtlasBounds0.w * input.texcoord.y)).rgb;

	if (input.alphavalues.x > 0.0)
	{
		terraincolor = u_Texture.Sample(u_Sampler, float2(u_AtlasBounds1.x + u_AtlasBounds1.z * input.texcoord.x,
			u_AtlasBounds1.y + u_AtlasBounds1.w * input.texcoord.y)).rgb
			* input.alphavalues.x + (terraincolor * (1.0 - input.alphavalues.x));
	}

	if (input.alphavalues.y > 0.0)
	{
		terraincolor = u_Texture.Sample(u_Sampler, float2(u_AtlasBounds2.x + u_AtlasBounds2.z * input.texcoord.x,
			u_AtlasBounds2.y + u_AtlasBounds2.w * input.texcoord.y)).rgb
			* input.alphavalues.y + (terraincolor * (1.0 - input.alphavalues.y));
	}

	if (input.alphavalues.z > 0.0)
	{
		terraincolor = u_Texture.Sample(u_Sampler, float2(u_AtlasBounds3.x + u_AtlasBounds3.z * input.texcoord.x,
			u_AtlasBounds3.y + u_AtlasBounds3.w * input.texcoord.y)).rgb
			* input.alphavalues.z + (terraincolor * (1.0 - input.alphavalues.z));
	}

	return float4(terraincolor, 1.0);
}