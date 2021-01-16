#version 330 core
layout (location = 0) in vec3 inPosition;

out vec3 exPosition;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    exPosition = inPosition;
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(inPosition, 1.0);
}