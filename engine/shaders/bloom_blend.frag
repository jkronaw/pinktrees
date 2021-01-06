#version 420 core

layout (location = 0) out vec4 outBloom;

in vec2 exTexcoord;

layout (binding = 0) uniform sampler2D gShaded;
layout (binding = 1) uniform sampler2D gBloom;
uniform float exposure;

void main()
{             
    vec3 hdrColor = texture(gShaded, exTexcoord).rgb;
    vec3 bloomColor = texture(gBloom, exTexcoord).rgb;
    bloomColor = bloomColor * 1.5 * exposure;
    vec3 result = hdrColor + bloomColor;

    outBloom = vec4(result, 1.0);
}  

