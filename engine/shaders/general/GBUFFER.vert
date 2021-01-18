#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexcoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTangent;

out vec2 exTexcoord;
out vec3 exNormal;
out vec4 exPosition;
out mat3 exTBN;

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

layout(shared) uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
    vec4 inPosition4 = vec4(inPosition, 1.0);

	exPosition = ModelMatrix * inPosition4;
	exTexcoord = inTexcoord;
	exNormal = inNormal;
	
	vec3 T = normalize(NormalMatrix * inTangent);	
    vec3 N = normalize(NormalMatrix * inNormal);
    vec3 B = normalize(NormalMatrix * cross(N, T));
	exTBN = mat3(T, B, N);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition, 1.0);
}
