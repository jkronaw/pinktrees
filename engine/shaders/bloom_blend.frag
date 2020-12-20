#version 420 core

layout (location = 0) out vec4 OutShaded;
layout (location = 1) out vec4 OutBloom;

in vec2 texcoord;

layout (binding = 0) uniform sampler2D gShaded;
layout (binding = 1) uniform sampler2D gBloom;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(gShaded, texcoord).rgb;      
    vec3 bloomColor = texture(gBloom, texcoord).rgb;
    bloomColor = bloomColor * 10.0 * exposure;
    vec3 result = hdrColor + bloomColor;
   
    OutShaded = vec4(hdrColor, 1.0);
    OutBloom = vec4(result, 1.0);
}  

