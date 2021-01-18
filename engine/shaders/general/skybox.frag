#version 330 core
in vec3 exTexcoord;

layout (location = 0) out vec4 outColor;

uniform bool toneMap = false;
uniform samplerCube cubemap;

void main()
{
    vec3 color = texture(cubemap, exTexcoord).rgb;

    if (toneMap) {
        color = color / (color + 1.0);
        outColor = vec4(pow(color, vec3(1.0/2.2)), 1);
    } else {    
        outColor = vec4(color, 1.0);
    }
    
    gl_FragDepth = 1.0;
}