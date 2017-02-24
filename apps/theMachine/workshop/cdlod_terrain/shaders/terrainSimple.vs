#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

uniform vec3 translation;
uniform float scale;
uniform float range;
uniform sampler2D heightMapTexture;
uniform vec3 gridDim;

float getHeight(vec2 v) 
{
	return 100.0*(texture( heightMapTexture, v/512.0).r - 0.5); 
}

void main(void)
{
	vec3 fWorldPosition = scale*position + translation;
	fWorldPosition.y = getHeight(fWorldPosition.xz);

    vec4 fPosition = view * vec4(fWorldPosition,1.0);

    gl_Position = projection * fPosition;
}
