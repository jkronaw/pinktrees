#version 430 core

in vec3 exPosition;
in vec2 exTexcoord;
in mat3 exTBN;

out vec4 FragmentColor;

// lights
const vec3 lightPositions[4] = { vec3(2, 3, 2), vec3(-2, 3,2), vec3(2, 3, -2), vec3(-2, 3, -2) };
const vec3 lightColors[4] = { vec3(15), vec3(15), vec3(15), vec3(15) };

uniform vec3 viewPos;

uniform sampler2D texAlbedo;
uniform sampler2D texNormal;
uniform sampler2D texRoughness;
uniform sampler2D texMetallic;
uniform sampler2D texAO;

const float PI = 3.14159265359;

float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1  = geometrySchlickGGX(NdotL, roughness);
    float ggx2  = geometrySchlickGGX(NdotV, roughness);	
    return ggx1 * ggx2;
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
} 

void main()
{	
    vec2 texcoord = vec2(exTexcoord.x, 1 - exTexcoord.y); // invert y coordinate
	vec3 normal  = texture(texNormal, texcoord).rgb * 2.0 - 1.0; // map into range [-1, 1]

	vec3 n = normalize(exTBN * normal);
    vec3 albedo  = texture(texAlbedo, texcoord).rgb;
    albedo = pow(albedo, vec3(2.2));
    float roughness = texture(texRoughness, texcoord).r;
    float metallic = texture(texMetallic, texcoord).r;
    float ao = texture(texAO, texcoord).r;

    // view vector
    vec3 w_0 = normalize(viewPos - exPosition);
    
    // base reflectivity considering metallic
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // direct lighting
    vec3 L_0 = vec3(0.0);
    for(int i = 0; i < 4; ++i) 
    {
        vec3 w_i = normalize(lightPositions[i] - exPosition); // light vector
        vec3 h = normalize(w_0 + w_i); // halfway vector

        // calculate attenuated light color (radiance)
        float distance = length(lightPositions[i] - exPosition);
        vec3 L_i = lightColors[i] / (distance * distance);

        // specular component
        float NDF = distributionGGX(n, h, roughness);
        float G = geometrySmith(n, w_0, w_i, roughness);
        vec3 F = fresnelSchlick(max(dot(h, w_0), 0.0), F0);  

        vec3 kS = F;
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(n, w_0), 0.0) * max(dot(n, w_i), 0.0);
        vec3 specular     = numerator / max(denominator, 0.001);  

        // diffuse component
        vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);
        vec3 diffuse = kD * albedo / PI;

        // calculate BRDF
        vec3 BRDF = diffuse + specular;

        // calculate reflection equation and add to L_0
        L_0 += BRDF * L_i * max(dot(n, w_i), 0.0);
    }

    // ambient lighting considering ao texture
    vec3 ambient = vec3(0.05) * albedo * ao;
    vec3 color = L_0 + ambient;

	// tone map from HDR to LDR
    color = color / (color + vec3(1.0)); 

    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    FragmentColor = vec4(color, 1.0);
}
