#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

out vec4 FragmentColor;

uniform sampler2D Texture0;

void main(void)
{
	//	vec3 color = vec3(1.0);
	//	vec3 color = (exPosition + vec3(1.0)) * 0.5;
	//vec3 color = vec3(exTexcoord, 0.0);
	//vec3 color = (exNormal + vec3(1.0)) * 0.5;

	//	vec3 normal = abs(exNormal);
	//	vec3 color3 = color * normal.z + normal.x * color * 0.5 + normal.y * color * 0.7;

	FragmentColor = texture(Texture0, exTexcoord);

//		vec3 N = normalize(exNormal);
//		vec3 direction = vec3(1.0, 0.5, 0.25);
//		float intensity = max(dot(direction, N), 0.0);
//		FragmentColor = vec4(vec3(intensity), 1.0);

}