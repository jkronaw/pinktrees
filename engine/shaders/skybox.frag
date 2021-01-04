#version 330 core

in vec3 texCoord;

layout (location = 0) out vec4 outColor;

uniform samplerCube cubemap;

void main()
{
    vec3 color = vec3(texture(cubemap, texCoord));

    outColor = vec4(color, 1.0);
}