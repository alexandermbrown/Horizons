#type vertex
#version 450 core

layout(location = 0) in mat4 I_TRANSFORM;
layout(location = 4) in vec4 I_ATLASBOUNDS;
layout(location = 5) in vec4 COLOR;

layout(location = 6) in vec2 POSITION;
layout(location = 7) in vec2 TEXCOORD;

layout(std140, binding = 0) uniform ViewProjectionMatrix
{
	mat4 u_ViewProj;
};

out vec4 v_AtlasBounds;
out vec4 v_Color;
out vec2 v_TexCoord;

void main()
{
	v_Color = COLOR;
	v_TexCoord = vec2(I_ATLASBOUNDS.x + I_ATLASBOUNDS.z * TEXCOORD.x,
		I_ATLASBOUNDS.y + I_ATLASBOUNDS.w * TEXCOORD.y);

	gl_Position = u_ViewProj * I_TRANSFORM * vec4(POSITION, 0.0, 1.0);
}

#type fragment
#version 450 core
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	color = v_Color * texture(u_Texture, v_TexCoord);
}