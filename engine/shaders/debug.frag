#version 330 core
in vec2 exTexcoord;

out vec4 FragmentColor;

uniform sampler2D brdfLUT;

void main()
{

    FragmentColor = vec4(texture(brdfLUT, exTexcoord).rgb,1);
}