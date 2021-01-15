#type vertex
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

#type fragment
#version 420 core
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

layout(binding = 0) uniform sampler2D u_Texture0;
layout(binding = 1) uniform sampler2D u_Texture1;
layout(binding = 2) uniform sampler2D u_Texture2;
layout(binding = 3) uniform sampler2D u_Texture3;
layout(binding = 4) uniform sampler2D u_Texture4;
layout(binding = 5) uniform sampler2D u_Texture5;
layout(binding = 6) uniform sampler2D u_Texture6;
layout(binding = 7) uniform sampler2D u_Texture7;

void main()
{
	vec4 tex_color = v_Color;
	switch(int(v_TexIndex))
	{
		case 0: tex_color *= texture(u_Texture0, v_TexCoord); break;
		case 1: tex_color *= texture(u_Texture1, v_TexCoord); break;
		case 2: tex_color *= texture(u_Texture2, v_TexCoord); break;
		case 3: tex_color *= texture(u_Texture3, v_TexCoord); break;
		case 4: tex_color *= texture(u_Texture4, v_TexCoord); break;
		case 5: tex_color *= texture(u_Texture5, v_TexCoord); break;
		case 6: tex_color *= texture(u_Texture6, v_TexCoord); break;
		case 7: tex_color *= texture(u_Texture7, v_TexCoord); break;
	}
	color = tex_color;
}