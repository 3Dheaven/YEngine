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
#include "CVkCommon.hpp"
#include "CVkDevice.hpp"

struct CVkBuffer
{
	CVkDevice mDevice;

	CVkBuffer(CVkDevice &device);
	~CVkBuffer();

	void CreateBuffer(VkBuffer &, VkBufferUsageFlags, uint32_t, VkMemoryPropertyFlags, VkDeviceMemory &);
	void CreateUniformBuffer(VkBuffer &, uint32_t, VkDeviceMemory &);
	VkBufferCreateInfo CreateBufferCreateInfo(uint64_t, VkBufferUsageFlags);
	void AllocateMemory(VkDeviceMemory &, VkBuffer &, VkMemoryPropertyFlags);
	VkMemoryAllocateInfo CreateMemoryAllocateInfo(VkBuffer &, VkMemoryPropertyFlags);
	void MapMemory(VkDeviceMemory, uint64_t, uint64_t, void **);
	uint32_t FindMemoryType(uint32_t, VkMemoryPropertyFlags);
};
