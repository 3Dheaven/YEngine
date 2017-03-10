#include "CVkShader.hpp"

void CVkShader::connectDevice(VkDevice & device)
{
	this->mLogicalDevice = device;
}

VkPipelineShaderStageCreateInfo CVkShader::CreatePipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule & module, const char * entryName) const noexcept
{
	VkPipelineShaderStageCreateInfo shaderStageInfo = {};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.stage = stage;
	shaderStageInfo.module = module;
	shaderStageInfo.pName = entryName;
	return shaderStageInfo;
}

VkPipelineVertexInputStateCreateInfo CVkShader::CreatePipelineVertexInputStateCreateInfo() const noexcept
{
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	/*vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;*/

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = mAttributeDescriptions.size();
	vertexInputInfo.pVertexBindingDescriptions = &mBindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = mAttributeDescriptions.data();

	return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo CVkShader::CreatePipelineInputAssemblyStateCreateInfo(const VkPrimitiveTopology & topology, uint32_t restartEnable) const noexcept
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = topology;
	inputAssembly.primitiveRestartEnable = restartEnable;
	return inputAssembly;
}

VkShaderModuleCreateInfo CVkShader::CreateShaderModuleCreateInfo(const std::vector<char>& code) const noexcept
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = (uint32_t*)code.data();
	return createInfo;
}

void CVkShader::CreateShaderModule(const std::vector<char>& code, VkShaderModule & shaderModule)
{
	VkShaderModuleCreateInfo createInfo = CreateShaderModuleCreateInfo(code);

	auto result = vkCreateShaderModule(mLogicalDevice, &createInfo, nullptr, &shaderModule);
	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Failed to create shader module:");
	}
}
