#include "ViewProjCB.h"
#include "TransformCB.h"

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

PS_IN vs_main(VS_IN_STATIC s_input, VS_IN_DYNAMIC d_input)
{
	PS_IN output;

	output.position = mul(u_ViewProj, mul(u_Transform, float4(s_input.position, 0.0, 1.0)));
	//output.position = mul(mul(float4(s_input.position, 0.0, 1.0), u_Transform), u_ViewProj);
	output.texcoord = s_input.texcoord;
	output.alphavalues = d_input.alphavalues;

	return output;
}
