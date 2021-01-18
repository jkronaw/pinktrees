#version 420 core

layout (location = 0) out vec4 outColor;

in vec2 exTexcoord;

layout (binding = 0) uniform sampler2D gReflection;
layout (binding = 1) uniform sampler2D gReflectionBlur;
layout (binding = 2) uniform sampler2D gMetallicRoughnessAO;
layout (binding = 3) uniform sampler2D gShaded;

void main()
{             
    // a channel contains visibility information of reflection
    vec4 reflectionColor = texture(gReflection, exTexcoord).rgba;
    vec3 reflectionBlurColor = texture(gReflectionBlur, exTexcoord).rgb;
    vec3 baseColor = texture(gShaded, exTexcoord).rgb;
	float roughness = clamp(1 - texture(gMetallicRoughnessAO, exTexcoord).g, 0, 1);

	vec3 ref = mix(reflectionBlurColor.rgb, reflectionColor.rgb, roughness);
    vec3 combined = mix(baseColor, ref, reflectionColor.a);

    outColor = vec4(combined,1 );
}  

