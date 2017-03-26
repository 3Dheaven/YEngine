#version 420 core
out vec4 outColor;

uniform vec4 custom_color;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 texDNS;
layout(binding=0) uniform sampler2D texture_diffuse;
//layout(binding=1) uniform sampler2D texture_specular;
//layout(binding=2) uniform sampler2D texture_normal;
in vec2 TexCoords;
void main()
{
	vec3 diff = texture2D(texture_diffuse, TexCoords).xyz;
	outColor = vec4(diff, 1.0) * custom_color;
}