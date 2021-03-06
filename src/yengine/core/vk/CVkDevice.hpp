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
//#include "CVkInstance.h"
#include "CVkCommon.hpp"
#include <set>

const std::vector<const char*> deviceExtensions = 
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete()
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

struct CVkDevice
{
	VkSurfaceKHR mSurface;
	VkInstance mInstance;
	VkPhysicalDevice mPhysicalDevice;
	VkDevice mLogicalDevice;
	VkQueue mGraphicsQueue;
	VkQueue mPresentQueue;
	VkSwapchainKHR mSwapChain;
	VkCommandPool mCommandPool;

	CVkDevice();
	~CVkDevice();

	void connectInstance(VkInstance &instance);
	void connectSwapChain(VkSwapchainKHR &swapChain);
	void connectSurface(VkSurfaceKHR &surface);

	void pickPhysicalDevice();
	void createLogicalDevice();

	// Check if a physical devices supports rendering to our surface and has a graphics queue.
	bool isDeviceSuitable(const VkPhysicalDevice& device) const;
	bool checkDeviceExtensionSupport(const VkPhysicalDevice& device) const;
	QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR &surface) const;
	SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device) const;

	/* 
	Command pools are opaque objects from where we allocate command buffers.
	They allow the Vulkan implementation to amortise the cost of resource creation across multiple command buffers.
	Command buffers are submitted to a work queue.
	*/
	void createCommandPool();

	void CopyBuffer(const VkBuffer &, VkBuffer &, VkDeviceSize);
	void CreateBuffer(VkBuffer &, VkBufferUsageFlags, uint32_t size, VkMemoryPropertyFlags properties, VkDeviceMemory &deviceMemorie);
	void AllocateMemory(VkDeviceMemory &deviceMemorie, VkBuffer &buffer, VkMemoryPropertyFlags properties);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};
