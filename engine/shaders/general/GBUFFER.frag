#version 430 core

in vec2 exTexcoord;
in vec3 exNormal;
in vec4 exPosition;
in mat3 exTBN;

layout (location = 0) out vec4 WorldPosOut;
layout (location = 1) out vec3 AlbedoOut;
layout (location = 2) out vec3 NormalOut;
layout (location = 3) out vec3 MetallicRoughnessAOOut;
layout (location = 4) out vec3 TexcoordOut;

uniform sampler2D texAlbedo;
uniform sampler2D texNormal;
uniform sampler2D texRoughness;
uniform sampler2D texMetallic;
uniform sampler2D texAO;

uniform vec3 albedo;
uniform vec3 normal;
uniform float roughness;
uniform float metallic;
uniform float ao;

uniform bool useAlbedoTex;
uniform bool useNormalTex;
uniform bool useRoughnessTex;
uniform bool useMetallicTex;
uniform bool useAoTex;

void main()
{
    // invert texcoord y
	vec2 texcoord = vec2(exTexcoord.x, 1 - exTexcoord.y);

    // albedo image
    AlbedoOut = useAlbedoTex ? texture(texAlbedo, texcoord).rgb : albedo;


    // metallic, roughness, ao image
    MetallicRoughnessAOOut.r = useMetallicTex ? texture(texMetallic, texcoord).r : metallic;
    MetallicRoughnessAOOut.g = useRoughnessTex ? texture(texRoughness, texcoord).r : roughness;
    MetallicRoughnessAOOut.b = useAoTex ? texture(texAO, texcoord).r : ao;

    // position image
    WorldPosOut = exPosition;

    // normal image
    vec3 normalTemp = useNormalTex ? texture(texNormal, texcoord).rgb * 2.0 - 1.0 : normal; // map into range [-1, 1]
    NormalOut = normalize(exTBN * normalTemp);

    // texcoord image
    TexcoordOut = vec3(texcoord, 0.0);
}
