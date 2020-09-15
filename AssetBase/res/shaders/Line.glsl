#type vertex
#version 420 core

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec4 COLOR;

layout(std140, binding = 0) uniform ViewProjectionMatrix
{
	mat4 u_ViewProj;
};

out vec4 v_Color;

void main()
{
	v_Color = COLOR;
	gl_Position = u_ViewProj * vec4(POSITION, 1.0);
}



#type fragment
#version 420 core
layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
	color = v_Color;
}