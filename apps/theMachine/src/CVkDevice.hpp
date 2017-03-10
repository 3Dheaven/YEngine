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
//#include "CVkInstance.h"
#include "CVkCommon.hpp"
#include <set>

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool IsComplete()
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

class CVkDevice
{

public:
	CVkDevice();
	~CVkDevice();

	VkSurfaceKHR mSurface;
	VkInstance mInstance;
	VkPhysicalDevice m_physicalDevice;
	VkDevice m_logicalDevice;
	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
	VkSwapchainKHR mSwapChain;

	void connectInstance(VkInstance &instance);
	void connectSwapChain(VkSwapchainKHR &swapChain);
	void connectSurface(VkSurfaceKHR &surface);

	void PickPhysicalDevice();
	void CreateLogicalDevice();
	bool IsDeviceSuitable(const VkPhysicalDevice& device) const;
	bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const;
	QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR &surface) const;
	SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device) const;

	// Maybe should be "const VkPhysicalDevice & getPhysicalDevice() const;"
	// The same for "getLogicalDevice()"
	VkPhysicalDevice & getPhysicalDevice();
	VkDevice & getLogicalDevice();
};
