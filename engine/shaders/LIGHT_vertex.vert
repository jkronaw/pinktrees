#version 330

layout (location = 0) in vec3 inPosition;

out vec2 texcoord;

uniform mat4 ModelMAtrix;
uniform mat4 NormalMatrix;

void main()
{
	gl_Position = vec4(inPosition, 1.0);
	texcoord = vec2(gl_Position.xy * 0.5 + 0.5);
}
