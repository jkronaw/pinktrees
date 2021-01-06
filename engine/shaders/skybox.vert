#version 330 core

layout (location = 0) in vec3 position;

out vec3 texCoord;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main()
{
	texCoord = position;
	mat3 viewMatrixNoTranslation = mat3(ViewMatrix);
    gl_Position = ProjectionMatrix * vec4(viewMatrixNoTranslation * position, 1.0);
}
