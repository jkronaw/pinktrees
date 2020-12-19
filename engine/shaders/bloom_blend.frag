#version 330 core
out vec4 OutShaded; 
out vec4 OutBloom;


in vec2 texcoord;

uniform sampler2D gShaded;
uniform sampler2D gBloom;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(gShaded, texcoord).rgb;      
    vec3 bloomColor = texture(gBloom, texcoord).rgb;
    bloomColor = bloomColor * exposure;
    vec3 result = hdrColor + bloomColor;
   
    OutShaded = vec4(result,1.0);
}  

