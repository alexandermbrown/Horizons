#type vertex
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 0.0, 1.0);
}



#type fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    vec2 pos = v_TexCoord.xy;
    vec3 sample = texture(u_Texture, v_TexCoord).rgb;
    ivec2 sz = textureSize(u_Texture, 0).xy;
    float dx = dFdx(pos.x) * sz.x; 
    float dy = dFdy(pos.y) * sz.y;
    float sigDist = median(sample.r, sample.g, sample.b);
    float w = fwidth(sigDist);
    float opacity = smoothstep(0.5 - w, 0.5 + w, sigDist);
    color = vec4(u_Color.rgb, opacity * u_Color.a);
}