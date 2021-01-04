#version 330 core

in vec3 texCoord;

layout (location = 0) out vec4 outColor;

uniform samplerCube cubemap;

void main()
{   
    gl_FragDepth = 1.0;
    vec3 color = vec3(texture(cubemap, texCoord));

    outColor = vec4(color, 1.0);
    outColor = vec4(pow(outColor.rgb, vec3(1.5)), 1.0);
}