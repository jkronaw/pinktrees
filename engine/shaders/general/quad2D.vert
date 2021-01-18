#version 330 core
layout (location = 0) in vec3 inPosition; // positions of the 2d quad corners

out vec2 exTexcoord;

void main()
{
	exTexcoord = vec2(inPosition.xy * 0.5 + 0.5); // transform from [-1, 1] to [0, 1]
	gl_Position = vec4(inPosition, 1.0);
}
