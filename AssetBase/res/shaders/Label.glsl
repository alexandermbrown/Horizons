#type vertex
#version 420 core

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec2 TEXCOORD;

layout(std140, binding = 0) uniform ViewProjectionMatrix
{
	mat4 u_ViewProj;
};

layout(std140, binding = 1) uniform TransformMatrix
{
	mat4 u_Transform;
};

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = TEXCOORD;
	gl_Position = u_ViewProj * u_Transform * vec4(POSITION, 1.0);
}



#type fragment
#version 420 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

layout(std140, binding = 2) uniform Font
{
	vec4 u_Color;
    float u_DistanceFactor;
};

uniform sampler2D u_Texture;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    vec3 msdfsample = texture(u_Texture, v_TexCoord).rgb;
    float sigDist = u_DistanceFactor * (median(msdfsample.r, msdfsample.g, msdfsample.b) - 0.5);
    float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
    color = vec4(u_Color.rgb, opacity * u_Color.a);
}