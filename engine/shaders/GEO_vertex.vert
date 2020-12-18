#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;
in vec3 inTangent;

out vec2 exTexcoord;
out vec3 exNormal;
out vec3 exPosition;
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

	exPosition = (ModelMatrix * inPosition4).xyz;
	exTexcoord = inTexcoord;
	exNormal = inNormal;
	
	vec3 T = normalize(NormalMatrix * inTangent);	
    vec3 N = normalize(NormalMatrix * inNormal);
    vec3 B = normalize(NormalMatrix * cross(N, T));
	exTBN = mat3(T, B, N);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition, 1.0);
}
