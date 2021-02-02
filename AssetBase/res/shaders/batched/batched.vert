#version 420 core

layout(location = 0) in vec3 POSITION;
layout(location = 1) in float TEXINDEX;
layout(location = 2) in vec2 TEXCOORD;
layout(location = 3) in vec4 COLOR;

layout(std140, binding = 0) uniform ViewProjectionMatrix
{
	mat4 u_ViewProj;
};

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{
	v_Color = COLOR;
	v_TexCoord = TEXCOORD;
	v_TexIndex = TEXINDEX;

	gl_Position = u_ViewProj * vec4(POSITION, 1.0);
}
