#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;
in vec3 inTangent;

out vec3 exPosition;
out vec2 exTexcoord;
out mat3 exTBN;

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
    vec4 inPosition4 = vec4(inPosition, 1.0);

	exPosition = (ModelMatrix * inPosition4).xyz;
	exTexcoord = inTexcoord;
	
	vec3 T = normalize(NormalMatrix * inTangent);	
    vec3 N = normalize(NormalMatrix * inNormal);
    vec3 B = normalize(NormalMatrix * cross(N, T));
	exTBN = mat3(T, B, N);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition, 1.0);
}
