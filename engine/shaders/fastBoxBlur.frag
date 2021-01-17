#version 420 core

layout (location = 0) out vec4 OutBlur;

uniform sampler2D gShaded;

in vec2 exTexcoord;

void main() {
   
    vec2 texSize  = textureSize(gShaded, 0).xy;
    int size = 3;
    int separation = 2;

    separation = max(separation, 1);

    OutBlur.rgb = vec3(0);

    float count = 0.0;

    for (int i = -size; i <= size; ++i) {
        for (int j = -size; j <= size; ++j) {
            OutBlur.rgb += texture(gShaded,( exTexcoord + (vec2(i, j) * separation)/texSize)).rgb;

            count += 1.0;
        }
    }

    OutBlur.rgb /= count;
}