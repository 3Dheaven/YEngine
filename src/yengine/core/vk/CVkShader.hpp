#pragma once

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
	void ConnectDevice(VkDevice &device);

	VkVertexInputBindingDescription mBindingDescription;
	std::vector<VkVertexInputAttributeDescription> mAttributeDescriptions;

	VkPipelineShaderStageCreateInfo CreatePipelineShaderStageCreateInfo(
        VkShaderStageFlagBits stage, VkShaderModule& module, const char* entryName) const noexcept;

	VkPipelineVertexInputStateCreateInfo CreatePipelineVertexInputStateCreateInfo() const noexcept;

	VkPipelineInputAssemblyStateCreateInfo CreatePipelineInputAssemblyStateCreateInfo(
        const VkPrimitiveTopology& topology, uint32_t restartEnable) const noexcept;

	VkShaderModuleCreateInfo CreateShaderModuleCreateInfo(const std::vector<char>& code) const noexcept;

	void CreateShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule);

private:
	VkDevice mLogicalDevice;
};
