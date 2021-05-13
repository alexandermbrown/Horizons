#include "AtlasBoundsCB.h"

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 alphavalues : ALPHAVALUES;
};

Texture2D u_Texture : register( t0 );
SamplerState u_TextureSampler : register( s0 );

Texture2D u_Noise1 : register( t1 );
SamplerState u_Noise1Sampler : register( s1 );

Texture2D u_Noise2 : register( t2 );
SamplerState u_Noise2Sampler : register( s2 );

Texture2D u_Noise3 : register( t3 );
SamplerState u_Noise3Sampler : register( s3 );

float width_blend(float alpha, float blendwidth)
{
	if (blendwidth == 0.0)
	{
		if (alpha < 0.5)
			return 0.0;
		else
			return 1.0;
	}
	else
		return saturate((1.0 / blendwidth) * (alpha - 0.5) + 0.5);
}

float4 ps_main(PS_IN input) : SV_TARGET
{
	float3 terraincolor = u_Texture.Sample(u_TextureSampler, float2(u_AtlasBounds0.x + u_AtlasBounds0.z * input.texcoord.x,
		u_AtlasBounds0.y + u_AtlasBounds0.w * input.texcoord.y)).rgb;

	float alphaoffset1 = (u_Noise1.Sample(u_Noise1Sampler, input.texcoord).r - 0.5) * u_NoiseWeights.x;
	float alpha1 = width_blend(input.alphavalues.x + alphaoffset1, u_BlendWidths.x);

	float alphaoffset2 = (u_Noise2.Sample(u_Noise2Sampler, input.texcoord).r - 0.5) * u_NoiseWeights.y;
	float alpha2 = width_blend(input.alphavalues.y + alphaoffset2, u_BlendWidths.y);

	float alphaoffset3 = (u_Noise3.Sample(u_Noise3Sampler, input.texcoord).r - 0.5) * u_NoiseWeights.z;
	float alpha3 = width_blend(input.alphavalues.z + alphaoffset3, u_BlendWidths.z);

	if (alpha1 > 0.0)
	{
		terraincolor = lerp(terraincolor, u_Texture.Sample(u_TextureSampler, float2(u_AtlasBounds1.x + u_AtlasBounds1.z * input.texcoord.x,
			u_AtlasBounds1.y + u_AtlasBounds1.w * input.texcoord.y)).rgb, alpha1);
	}

	if (alpha2 > 0.0)
	{
		terraincolor = lerp(terraincolor, u_Texture.Sample(u_TextureSampler, float2(u_AtlasBounds2.x + u_AtlasBounds2.z * input.texcoord.x,
			u_AtlasBounds2.y + u_AtlasBounds2.w * input.texcoord.y)).rgb, alpha2);
	}

	if (alpha3 > 0.0)
	{
		terraincolor = lerp(terraincolor, u_Texture.Sample(u_TextureSampler, float2(u_AtlasBounds3.x + u_AtlasBounds3.z * input.texcoord.x,
			u_AtlasBounds3.y + u_AtlasBounds3.w * input.texcoord.y)).rgb, alpha3);
	}

	return float4(terraincolor, 1.0);
}
