#version 330 core
in vec3 position;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main()
{
	//gl_Position = vec4(position, 0.0, 1.0);
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
}