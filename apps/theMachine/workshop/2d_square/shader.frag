#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform myUniformBuffer1
{
	vec4 myElement;
}ubo1;

layout(set = 0, binding = 1) uniform myUniformBuffer2
{
	vec4 myElement1;
}ubo2;

layout(location = 0) out vec4 outColor;

void main()
{
	//outColor = vec4(ubo2.myElement1.rgb, 1.0);
	//outColor = vec4(ubo1.myElement.rgb, 1.0);
	outColor = vec4(ubo2.myElement1.rgb, 1.0) + vec4(ubo1.myElement.rgb, 1.0);
}