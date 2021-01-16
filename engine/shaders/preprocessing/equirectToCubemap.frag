#version 330 core

// position on cube in world space
in vec3 exPosition;

out vec4 FragmentColor;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(exPosition));
    vec3 color = texture(equirectangularMap, uv).rgb;
    FragmentColor = vec4(color, 1.0);
}
