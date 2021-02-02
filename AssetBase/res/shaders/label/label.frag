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
