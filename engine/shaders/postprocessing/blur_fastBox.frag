#version 420 core

layout (location = 0) out vec4 OutBlur;

uniform sampler2D gShaded;

uniform int kernelSize;
uniform int kernelSeparation;

in vec2 exTexcoord;

void main() {
   
    vec2 texSize  = textureSize(gShaded, 0).xy;

    int separation = max(kernelSeparation, 1);

    OutBlur.rgb = vec3(0);

    float count = 0.0;

    for (int i = -kernelSize; i <= kernelSize; ++i) {
        for (int j = -kernelSize; j <= kernelSize; ++j) {
            OutBlur.rgb += texture(gShaded,( exTexcoord + (vec2(i, j) * separation)/texSize)).rgb;

            count += 1.0;
        }
    }

    OutBlur.rgb /= count;
}