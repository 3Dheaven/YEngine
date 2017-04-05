#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex
{
    vec4 gl_Position;
};

layout(std140, set = 0, binding = 1) uniform block
{
	mat4 mvp;
}ubo;

layout(location = 0) in vec4 inPosition;

void main()
{
    gl_Position = ubo.mvp * inPosition;

}