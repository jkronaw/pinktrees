#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in mat3 exTBN;

out vec4 FragmentColor;

uniform sampler2D texAlbedo;
uniform sampler2D texNormal;

uniform vec3 viewPos;

const vec3 lightColor = vec3(1.0, 1.0, 1.0); // configure all lights to shine white

void main(void)
{
	vec3 albedo  = texture(texAlbedo, exTexcoord).rgb;
	vec3 normal  = texture(texNormal, exTexcoord).rgb * 2.0 - 1.0; // map into range [-1, 1]

	normal = normalize(exTBN * normal);

	float ambientStrength = 0.2f;
	vec3 ambient = lightColor * ambientStrength;

	vec3 lightPos = vec3(3, 3, 3);
	vec3 lightDir = normalize(lightPos - exPosition);
	float diffuseStrength = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = lightColor * diffuseStrength;

	vec3 viewDir = normalize(viewPos - exPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = lightColor * specularStrength;

	vec3 result = (ambient + diffuse + specular) * albedo;
	FragmentColor = vec4(result, 1.0);
}