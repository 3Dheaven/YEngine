#version 330 core
out vec4 outColor;

uniform vec4 custom_color;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 texDNS;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
in vec2 TexCoords;
void main()
{
	vec3 diff = texture2D(texture_diffuse, TexCoords).xyz;
	outColor = vec4(diff, 1.0);
}