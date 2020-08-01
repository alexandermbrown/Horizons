#type vertex
#version 450 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_AlphaValues;

layout(std140, binding = 0) uniform ViewProjectionMatrix
{
	mat4 u_ViewProj;
};

layout(std140, binding = 1) uniform TransformMatrix
{
	mat4 u_Transform;
};

out vec2 v_TexCoord;
out vec4 v_AlphaValues;

void main()
{
	v_TexCoord = a_TexCoord;
	v_AlphaValues = a_AlphaValues;

	gl_Position = u_ViewProj * u_Transform * vec4(a_Position, 0.0, 1.0);
}




#type fragment
#version 450 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_AlphaValues;

layout(std140, binding = 3) uniform AtlasBounds
{
	vec4 a_AtlasBounds0;
	vec4 a_AtlasBounds1;
	vec4 a_AtlasBounds2;
	vec4 a_AtlasBounds3;
};

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, vec2(v_AtlasBounds0.x + v_AtlasBounds0.z * v_TexCoord.x, v_AtlasBounds0.y + v_AtlasBounds0.w * v_TexCoord.y));

	if (v_AlphaValues.x > 0.0)
	{
		color = texture(u_Texture, vec2(v_AtlasBounds1.x + v_AtlasBounds1.z * v_TexCoord.x, v_AtlasBounds1.y + v_AtlasBounds1.w * v_TexCoord.y)
			* v_AlphaValues.x) + (color * (1.0 - v_AlphaValues.x));
	}

	if (v_AlphaValues.y > 0.0)
	{
		color = texture(u_Texture, vec2(v_AtlasBounds2.x + v_AtlasBounds2.z * v_TexCoord.x, v_AtlasBounds2.y + v_AtlasBounds2.w * v_TexCoord.y)
			* v_AlphaValues.x) + (color * (1.0 - v_AlphaValues.x));
	}

	if (v_AlphaValues.z > 0.0)
	{
		color = texture(u_Texture, vec2(v_AtlasBounds3.x + v_AtlasBounds3.z * v_TexCoord.x, v_AtlasBounds3.y + v_AtlasBounds3.w * v_TexCoord.y)
			* v_AlphaValues.x) + (color * (1.0 - v_AlphaValues.x));
	}
}