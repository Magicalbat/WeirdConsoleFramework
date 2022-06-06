#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aTranslate;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec3 aColor;

uniform vec2 uUvStep;
uniform mat4 uModelProj;

out vec2 uv;
out vec3 color;

void main()
{
	uv = aUV + vec2(uUvStep.x * float(gl_VertexID == 1 || gl_VertexID == 2), uUvStep.y * float(gl_VertexID > 1));
	color = aColor;

	gl_Position = vec4(aPos.xy + aTranslate, 0, 1) * uModelProj;
}