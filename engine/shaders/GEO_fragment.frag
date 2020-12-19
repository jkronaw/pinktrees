#version 430 core

in vec2 exTexcoord;
in vec3 exNormal;
in vec3 exPosition;
in mat3 exTBN;

layout (location = 0) out vec3 WorldPosOut;
layout (location = 1) out vec3 AlbedoOut;
layout (location = 2) out vec3 NormalOut;
layout (location = 3) out vec3 MetallicRoughnessAOOut;
layout (location = 4) out vec3 TexcoordOut;

uniform sampler2D texAlbedo;
uniform sampler2D texNormal;
uniform sampler2D texRoughness;
uniform sampler2D texMetallic;
uniform sampler2D texAO;


void main()
{	
   vec2 texcoord = vec2(exTexcoord.x, 1 - exTexcoord.y);
   WorldPosOut = exPosition;
   AlbedoOut = texture(texAlbedo, texcoord).rgb;
   vec3 normal = texture(texNormal, texcoord).rgb * 2.0 - 1.0; // map into range [-1, 1]
   NormalOut = normalize(exTBN * normal);
   TexcoordOut = vec3(texcoord, 0.0);
   MetallicRoughnessAOOut.r = texture(texMetallic, texcoord).r;
   MetallicRoughnessAOOut.g = texture(texRoughness, texcoord).r;
   MetallicRoughnessAOOut.b = texture(texAO, texcoord).r;
}
