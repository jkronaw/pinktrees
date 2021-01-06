#version 330 core

layout (location = 0) in vec3 inPosition;

out vec3 exTexcoord;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main()
{
	exTexcoord = inPosition;
	mat3 viewMatrixNoTranslation = mat3(ViewMatrix);
    gl_Position = ProjectionMatrix * vec4(viewMatrixNoTranslation * inPosition, 1.0);
}
