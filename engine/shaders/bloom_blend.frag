#version 420 core

layout (location = 0) out vec4 OutBloom;

in vec2 texcoord;

layout (binding = 0) uniform sampler2D gShaded;
layout (binding = 1) uniform sampler2D gBloom;
uniform float exposure;

void main()
{             
    vec3 hdrColor = texture(gShaded, texcoord).rgb;      
    vec3 bloomColor = texture(gBloom, texcoord).rgb;
    bloomColor = bloomColor * 1.5 * exposure;
    vec3 result = hdrColor + bloomColor;

    OutBloom = vec4(result, 1.0);
}  

