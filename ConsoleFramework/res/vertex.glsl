#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aColor;

uniform mat4 uModelProj;

out vec2 uv;
out vec3 color;

void main()
{
	uv = aUV;
	color = aColor;

	gl_Position = vec4(aPos.xy, 0, 1) * uModelProj;
}