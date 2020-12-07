#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;
in vec3 inTangent;

out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;

out mat3 exTBN;

uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;
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
	exNormal = inNormal;
	
	vec3 T = normalize(vec3(ModelMatrix * vec4(inTangent, 1.0)));	
    vec3 N = normalize(vec3(ModelMatrix * vec4(inNormal, 1.0)));
    vec3 B = normalize(vec3(ModelMatrix * vec4(cross(N, T), 1.0)));
	exTBN = mat3(T, B, N);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * inPosition4;
}
