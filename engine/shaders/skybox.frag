#version 330 core

in vec3 texCoord;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outBloom;

uniform samplerCube cubemap;

void main()
{
    outColor = vec3(texture(cubemap, texCoord));
}