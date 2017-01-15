#version 330 core
out vec4 outColor;

uniform vec3 custom_color;

void main()
{
	outColor = vec4(custom_color, 1.0);
}