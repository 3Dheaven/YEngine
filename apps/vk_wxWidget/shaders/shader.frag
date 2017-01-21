#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

layout (set=0, binding=0) uniform myUniformBuffer
{
	vec4 myElement;
};

void main()
{
    outColor = vec4(fragColor, 1.0);
}