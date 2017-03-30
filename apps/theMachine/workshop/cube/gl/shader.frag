#version 440 core

out vec4 outColor;
uniform vec4 custom_color;

void main()
{
	outColor = custom_color;
}
