#version 430 core

layout (location = 0) out vec4 outBloom;

in vec2 texcoord;

uniform sampler2D gShaded;

void main()
{
    // bloom separation

    vec3 color = texture(gShaded, texcoord).rgb;
    //vec3 threshold = vec3(0.5126, 0.4152, 0.352);
    vec3 threshold = vec3(0.0126, 0.0152, 1.52);
    float brightness = dot(color, threshold);
    if(brightness > 1.0)
        outBloom = vec4(color, 1.0);
    else
        outBloom = vec4(0.0, 0.0, 0.0, 1.0);

}