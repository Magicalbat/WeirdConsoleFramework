#version 330 core

in vec2 uv;
in vec3 color;

uniform sampler2D image;

void main()
{
	gl_FragColor = texture(image, uv).x * vec4(color, 1);
}