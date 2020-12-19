#version 430 core

out vec3 outColor;
out vec3 outBloom;

in vec2 texcoord;

// GBuffer
uniform sampler2D gPosition;
uniform sampler2D gAlbedo;
uniform sampler2D gNormal;
uniform sampler2D gMetallicRoughnessAO;
uniform sampler2D gTexCoord;

uniform vec2 gScreenSize;
uniform vec3 viewPos;

// lights
const vec3 lightPositions[4] = { vec3(2, 3, 2), vec3(-2, 3,2), vec3(2, 3, -2), vec3(-2, 3, -2) };
const vec3 lightColors[4] = { vec3(15), vec3(15), vec3(15), vec3(15) };

const float PI = 3.14159265359;

// normal distribution function
float trowbridgeReitzGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

// used within geometry function
float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

// geometry function
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1  = geometrySchlickGGX(NdotL, roughness);
    float ggx2  = geometrySchlickGGX(NdotV, roughness);	
    return ggx1 * ggx2;
}

// fresnel term
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
} 

void main()
{
    vec3 n = (texture(gNormal, texcoord).rgb);
    vec3 position = texture(gPosition, texcoord).rgb;

    vec3 albedo  = texture(gAlbedo, texcoord).rgb;
    float metallic = texture(gMetallicRoughnessAO, texcoord).r;
    float roughness = texture(gMetallicRoughnessAO, texcoord).g;
    float ao = texture(gMetallicRoughnessAO, texcoord).b;

    // gamma correct albedo to get into linear space
    albedo = pow(albedo, vec3(2.2));

    // view vector
    vec3 w_0 = normalize(viewPos - position);
    
    // base reflectivity considering metallic
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // direct lighting
    vec3 L_0 = vec3(0.0);
    for(int i = 0; i < 4; ++i) 
    {
        vec3 w_i = normalize(lightPositions[i] - position); // light vector
        vec3 h = normalize(w_0 + w_i); // halfway vector

        // calculate attenuated light color (radiance)
        float distance = length(lightPositions[i] - position);
        vec3 L_i = lightColors[i] / (distance * distance);

        // specular component
        float NDF = trowbridgeReitzGGX(n, h, roughness);
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

    // bloom separation
    vec3 threshold = vec3(0.7, 0.7152, 0.7);
    float brightness = dot(color, threshold);
    if(brightness > 1.0)
        outBloom = vec3(color);
    else
        outBloom = vec3(0.0, 0.0, 0.0);

    outColor = vec3(color);
}
