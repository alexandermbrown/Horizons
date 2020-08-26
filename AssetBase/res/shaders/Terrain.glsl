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

layout(std140, binding = 3) uniform AtlasBounds
{
	vec4 u_AtlasBounds0;
	vec4 u_AtlasBounds1;
	vec4 u_AtlasBounds2;
	vec4 u_AtlasBounds3;
};

uniform sampler2D u_Texture;

void main()
{
	vec3 terraincolor = texture(u_Texture, vec2(u_AtlasBounds0.x + u_AtlasBounds0.z * v_TexCoord.x,
		u_AtlasBounds0.y + u_AtlasBounds0.w * v_TexCoord.y)).rgb;

	if (v_AlphaValues.x > 0.0)
	{
		terraincolor = texture(u_Texture, vec2(u_AtlasBounds1.x + u_AtlasBounds1.z * v_TexCoord.x,
			u_AtlasBounds1.y + u_AtlasBounds1.w * v_TexCoord.y)).rgb
			* v_AlphaValues.x + (terraincolor * (1.0 - v_AlphaValues.x));
	}

	if (v_AlphaValues.y > 0.0)
	{
		terraincolor = texture(u_Texture, vec2(u_AtlasBounds2.x + u_AtlasBounds2.z * v_TexCoord.x,
		u_AtlasBounds2.y + u_AtlasBounds2.w * v_TexCoord.y)).rgb
			* v_AlphaValues.y + (terraincolor * (1.0 - v_AlphaValues.y));
	}

	if (v_AlphaValues.z > 0.0)
	{
		terraincolor = texture(u_Texture, vec2(u_AtlasBounds3.x + u_AtlasBounds3.z * v_TexCoord.x,
		u_AtlasBounds3.y + u_AtlasBounds3.w * v_TexCoord.y)).rgb
			* v_AlphaValues.z + (terraincolor * (1.0 - v_AlphaValues.z));
	}

	color = vec4(terraincolor, 1.0);
}