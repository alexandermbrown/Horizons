#type vertex
#version 420 core

layout(location = 0) in vec2 POSITION;
layout(location = 1) in vec2 TEXCOORD;
layout(location = 2) in vec3 ALPHAVALUES;

layout(std140, binding = 0) uniform ViewProjectionMatrix
{
	mat4 u_ViewProj;
};

layout(std140, binding = 1) uniform TransformMatrix
{
	mat4 u_Transform;
};

out vec2 v_TexCoord;
out vec3 v_AlphaValues;

void main()
{
	v_TexCoord = TEXCOORD;
	v_AlphaValues = ALPHAVALUES;

	gl_Position = u_ViewProj * u_Transform * vec4(POSITION, 0.0, 1.0);
}




#type fragment
#version 420 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_AlphaValues;

layout(std140, binding = 3) uniform Terrain
{
	vec4 u_AtlasBounds0;
	vec4 u_AtlasBounds1;
	vec4 u_AtlasBounds2;
	vec4 u_AtlasBounds3;
	vec3 u_NoiseWeights;
	vec3 u_BlendWidths;
};

uniform sampler2D u_Texture;
uniform sampler2D u_Noise1;
uniform sampler2D u_Noise2;
uniform sampler2D u_Noise3;

float width_blend(float alpha, float blendwidth)
{
	if (blendwidth == 0.0)
	{
		if (alpha < 0.5)
		{
			return 0.0;
		}
		else
		{
			return 1.0;
		}
	}
	else
	{
		return clamp((1.0 / blendwidth) * (alpha - 0.5) + 0.5, 0.0, 1.0);
	}
}

void main()
{
	vec3 terraincolor = texture(u_Texture, vec2(u_AtlasBounds0.x + u_AtlasBounds0.z * v_TexCoord.x,
		u_AtlasBounds0.y + u_AtlasBounds0.w * v_TexCoord.y)).rgb;

	float alphaoffset1 = (texture(u_Noise1, v_TexCoord).r - 0.5) * u_NoiseWeights.x;
	float alpha1 = width_blend(v_AlphaValues.x + alphaoffset1, u_BlendWidths.x);

	float alphaoffset2 = (texture(u_Noise2, v_TexCoord).r - 0.5) * u_NoiseWeights.y;
	float alpha2 = width_blend(v_AlphaValues.y + alphaoffset2, u_BlendWidths.y);

	float alphaoffset3 = (texture(u_Noise3, v_TexCoord).r - 0.5) * u_NoiseWeights.z;
	float alpha3 = width_blend(v_AlphaValues.z + alphaoffset3, u_BlendWidths.z);
	
	if (alpha1 > 0.0)
	{
		terraincolor = mix(terraincolor, texture(u_Texture, vec2(u_AtlasBounds1.x + u_AtlasBounds1.z * v_TexCoord.x,
			u_AtlasBounds1.y + u_AtlasBounds1.w * v_TexCoord.y)).rgb, alpha1);
	}

	if (alpha2 > 0.0)
	{
		terraincolor = mix(terraincolor, texture(u_Texture, vec2(u_AtlasBounds2.x + u_AtlasBounds2.z * v_TexCoord.x,
			u_AtlasBounds2.y + u_AtlasBounds2.w * v_TexCoord.y)).rgb, alpha2);
	}

	if (alpha3 > 0.0)
	{
		terraincolor = mix(terraincolor, texture(u_Texture, vec2(u_AtlasBounds3.x + u_AtlasBounds3.z * v_TexCoord.x,
			u_AtlasBounds3.y + u_AtlasBounds3.w * v_TexCoord.y)).rgb, alpha3);
	}

	color = vec4(terraincolor, 1.0);
}