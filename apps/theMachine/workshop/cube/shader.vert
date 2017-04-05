#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex
{
    vec4 gl_Position;
};

layout(binding = 1) uniform myUniformMatrixBuffer
{
	mat4 myElement;
}uboVS;

layout(location = 0) in vec3 inPosition;

void main()
{
    gl_Position = uboVS.myElement * vec4(inPosition, 1.0);

}