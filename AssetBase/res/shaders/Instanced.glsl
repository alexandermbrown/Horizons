#type vertex
#version 450 core

layout(location = 0) in mat4 a_Transform;
layout(location = 4) in vec4 a_AtlasBounds;
layout(location = 5) in vec4 a_Color;

layout(location = 6) in vec2 a_Position;
layout(location = 7) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform ViewProjectionMatrix
{
	mat4 u_ViewProj;
};

out vec4 v_AtlasBounds;
out vec4 v_Color;
out vec2 v_TexCoord;

void main()
{
	v_AtlasBounds = a_AtlasBounds;
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;

	gl_Position = u_ViewProj * a_Transform * vec4(a_Position, 0.0, 1.0);
}

#type fragment
#version 450 core
layout(location = 0) out vec4 color;

in vec4 v_AtlasBounds;
in vec4 v_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	color = v_Color * texture(
		u_Texture, 
		vec2(v_AtlasBounds.x + v_AtlasBounds.z * v_TexCoord.x, v_AtlasBounds.y + v_AtlasBounds.w * v_TexCoord.y)
	);
}