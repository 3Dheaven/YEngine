#version 330 core

layout (location = 0) in vec3 position;
uniform mat4 projection;
uniform mat4 view;
uniform vec4 color;
uniform vec3 translation;
uniform float scale;

out vec4 fPosition;
out vec3 fWorldPosition;
out vec3 fColor;

void main()
{

	fColor = vec3(color.x, color.y, color.z);
	fWorldPosition = scale*position + translation;

	//height!
	fWorldPosition.y = 0.0; 

    fPosition = view * vec4(fWorldPosition,1.0);

    gl_Position = projection * fPosition;
	

}  