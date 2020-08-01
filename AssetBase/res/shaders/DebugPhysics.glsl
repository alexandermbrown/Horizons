#type vertex
#version 450 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec4 a_Color;

layout(std140, binding = 0) uniform ViewProjectionMatrix
{
	mat4 u_ViewProj;
};

out vec4 v_Color;

void main()
{
	v_Color = a_Color;
	gl_Position = u_ViewProj * vec4(a_Position, 0.4, 1.0);
}



#type fragment
#version 450 core
layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
	color = v_Color;
}