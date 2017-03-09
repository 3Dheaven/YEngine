
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

struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool IsComplete()
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

static QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR &surface)
{
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) 
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
		{
			indices.graphicsFamily = i;
		}
		VkBool32 presentSupport = false;
		VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (result != VK_SUCCESS) 
		{
			throw CVulkanException(result, "Error while attempting to check if a surface supports presentation:");
		}
		if (queueFamily.queueCount > 0 && presentSupport) 
		{
			indices.presentFamily = i;
		}
		if (indices.IsComplete()) 
		{
			break;
		}
		++i;
	}
	return indices;
}

