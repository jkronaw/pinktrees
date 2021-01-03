#version 330 core

layout (location = 0) in vec3 position;

out vec3 texCoord;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform vec3 viewPos;

void main()
{
	texCoord = position;
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(position + viewPos, 1.0);
}
