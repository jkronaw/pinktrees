#version 430 core

layout (location = 0) out vec4 outBloom;

in vec2 exTexcoord;

uniform sampler2D gShaded;

uniform float bloomThreshold;

void main()
{
    // bloom separation

    vec3 color = texture(gShaded, exTexcoord).rgb;
    //vec3 threshold = vec3(0.5126, 0.4152, 0.352) * bloomThreshold;
    
    vec3 threshold = vec3(0.0126, 0.0152, 1.52) * bloomThreshold;
    
    // alternative threshold calculation:
    //float treshVal = max(color.r, max(color.g, color.b));
    //if (treshVal < bloomThreshold) { color = vec3(0.0); }
    //outBloom = vec4(color,1);

    float brightness = dot(color, threshold);
    if(brightness > 1.0)
        outBloom = vec4(color, 1.0);
    else
        outBloom = vec4(0.0, 0.0, 0.0, 1.0);

}