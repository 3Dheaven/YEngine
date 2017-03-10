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
	VkBuffer mBuffer;
	VkDeviceMemory mDeviceMemory;

	CVkBuffer(CVkDevice &device);
	~CVkBuffer();

	void CreateBuffer(VkBufferUsageFlags, uint32_t, VkMemoryPropertyFlags);
	void CreateUniformBuffer(uint32_t);
	VkBufferCreateInfo CreateBufferCreateInfo(uint64_t, VkBufferUsageFlags);
	void AllocateMemory(VkMemoryPropertyFlags);
	VkMemoryAllocateInfo CreateMemoryAllocateInfo(VkMemoryPropertyFlags);
	void MapMemory(uint64_t, uint64_t, void **);
	uint32_t FindMemoryType(uint32_t, VkMemoryPropertyFlags);
};