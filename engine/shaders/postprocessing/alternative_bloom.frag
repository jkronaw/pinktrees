#version 410

uniform sampler2D gShaded;

uniform float bloomThreshold;

out vec4 outBloom;

in vec2 exTexcoord;
void main() {
  int   size      = 4;
  float separation = 2;//4.0;
  float threshold  = bloomThreshold;
  float amount     = 0.6;

  vec2 texSize = textureSize(gShaded, 0).xy;

  vec4 result = vec4(0.0);
  vec4 color  = vec4(0.0);

  float treshVal = 0.0;
  int count = 0;

  for (int i = -size; i <= size; ++i) {
    for (int j = -size; j <= size; ++j) {
      color = texture(gShaded, (exTexcoord + (vec2(i, j) * separation)/texSize));
      treshVal = max(color.r, max(color.g, color.b));
      if (treshVal < threshold) { color = vec4(0.0); }

      result += color;
      count++;
    }
  }

  result /= count;

  outBloom = mix(vec4(0.0), result, 1);
}