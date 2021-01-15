#version 430 core

layout (location = 0) out vec4 outColor;

// texcoord from screen space quad
in vec2 exTexcoord;

// GBuffer
uniform sampler2D gPosition;
uniform sampler2D gAlbedo;
uniform sampler2D gNormal;
uniform sampler2D gMetallicRoughnessAO;
uniform sampler2D gTexCoord;

uniform vec2 gScreenSize;
uniform vec3 viewPos;

// direct lighting
#define MAX_LIGHT_COUNT 128
uniform int lightCount;
uniform vec3 lightPositions[MAX_LIGHT_COUNT];
uniform vec3 lightColors[MAX_LIGHT_COUNT];

// image based lighting
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;

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

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

void main()
{
    vec3 n = (texture(gNormal, exTexcoord).rgb);
    vec3 position = texture(gPosition, exTexcoord).rgb;

    vec3 albedo  = texture(gAlbedo, exTexcoord).rgb;
    float metallic = texture(gMetallicRoughnessAO, exTexcoord).r;
    float roughness = texture(gMetallicRoughnessAO, exTexcoord).g;
    float ao = texture(gMetallicRoughnessAO, exTexcoord).b;

    // gamma correct albedo to get into linear space
    albedo = pow(albedo, vec3(2.2));

    // view vector
    vec3 w_0 = normalize(viewPos - position);
    
    // base reflectivity considering metallic
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // direct lighting
    vec3 L_0 = vec3(0.0);

    if(position != vec3(0,0,0)){

        for(int i = 0; i < lightCount; ++i) 
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
    }

    // image based lighting
    vec3 F = fresnelSchlickRoughness(max(dot(n, w_0), 0.0), F0, roughness);

    vec3 R = reflect(-w_0, n);  

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
  
    vec3 irradiance = texture(irradianceMap, n).rgb;
    vec3 diffuse    = irradiance * albedo;
  
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;   
    vec2 envBRDF  = texture(brdfLUT, vec2(max(dot(n, w_0), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
  
    vec3 ambient = (kD * diffuse + specular) * ao; 

    vec3 color = L_0 + ambient;

    // tone map from HDR to LDR
    color = color / (color + vec3(1.0));

    // gamma correct
    color = pow(color, vec3(1.0/2.2));

//    color = texture(brdfLUT, exTexcoord).rgb;
    outColor = vec4(color, 1.0);
}
