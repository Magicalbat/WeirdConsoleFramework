#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 colorIn;

out vec3 color;

void main()
{
	color = colorIn;

	gl_Position = position;
}