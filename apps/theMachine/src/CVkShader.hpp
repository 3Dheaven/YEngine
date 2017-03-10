#pragma once
#include "wx/wxprec.h"
#include <stdlib.h>
#include <string>
#include <fstream>
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <windows.h>
#include <vulkan.h>
#include "CVulkanException.h"

class CVkShader
{

public:

	VkDevice mLogicalDevice;

	void connectDevice(VkDevice &device)
	{
		this->mLogicalDevice = device;
	}

	VkVertexInputBindingDescription m_bindingDescription;
	std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;


	VkPipelineShaderStageCreateInfo CreatePipelineShaderStageCreateInfo(
		VkShaderStageFlagBits stage, VkShaderModule& module, const char* entryName) const noexcept
	{
		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = stage;
		shaderStageInfo.module = module;
		shaderStageInfo.pName = entryName;
		return shaderStageInfo;
	}

	VkPipelineVertexInputStateCreateInfo CreatePipelineVertexInputStateCreateInfo() const noexcept
	{
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		/*vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;*/

		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = m_attributeDescriptions.size();
		vertexInputInfo.pVertexBindingDescriptions = &m_bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = m_attributeDescriptions.data();

		return vertexInputInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo CreatePipelineInputAssemblyStateCreateInfo(
		const VkPrimitiveTopology& topology, uint32_t restartEnable) const noexcept
	{
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = topology;
		inputAssembly.primitiveRestartEnable = restartEnable;
		return inputAssembly;
	}

	VkShaderModuleCreateInfo CreateShaderModuleCreateInfo(
		const std::vector<char>& code) const noexcept
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = (uint32_t*)code.data();
		return createInfo;
	}

	void CreateShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule)
	{
		VkShaderModuleCreateInfo createInfo = CreateShaderModuleCreateInfo(code);

		auto result = vkCreateShaderModule(mLogicalDevice, &createInfo, nullptr, &shaderModule);
		if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Failed to create shader module:");
		}
	}
};
