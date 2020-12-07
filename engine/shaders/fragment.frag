#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

out vec4 FragmentColor;

uniform sampler2D albedo;
uniform sampler2D normal;

uniform vec3 viewPos;

void main(void)
{

	//	vec3 color = vec3(1.0);
	//	vec3 color = (exPosition + vec3(1.0)) * 0.5;
	//vec3 color = vec3(exTexcoord, 0.0);
	//vec3 color = (exNormal + vec3(1.0)) * 0.5;

	//	vec3 normal = abs(exNormal);
	//	vec3 color3 = color * normal.z + normal.x * color * 0.5 + normal.y * color * 0.7;

	vec3 lightColor = vec3(1, 1, 1);

	float ambientStrength = 0.2f;
	vec3 ambient = lightColor * ambientStrength;

	vec3 lightPos = vec3(3, 3, 3);
	vec3 lightDir = normalize(lightPos - exPosition);
	vec3 normal = normalize(exNormal);
	float diffuseStrength = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = lightColor * diffuseStrength;

	vec3 viewDir = normalize(viewPos - exPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	vec3 specular = lightColor * specularStrength;

	vec3 texColor = texture(albedo, exTexcoord).xyz;

	vec3 result = (ambient + diffuse + specular) * texColor;
	FragmentColor = vec4(result, 1.0);

//		vec3 N = normalize(exNormal);
//		vec3 direction = vec3(1.0, 0.5, 0.25);
//		float intensity = max(dot(direction, N), 0.0);
//		FragmentColor = vec4(vec3(intensity), 1.0);

}