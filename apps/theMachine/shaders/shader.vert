#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;
//in vec3 color;
//out vec3 Color;
out vec2 TexCoords;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main()
{
	//gl_Position = vec4(position, 0.0, 1.0);
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
	//Color = color;
	TexCoords = texCoords;
}