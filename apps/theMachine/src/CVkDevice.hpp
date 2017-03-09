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
#include "CVkInstance.h"

class CVkDevice
{
public:
	CVkDevice(CVkInstance &instance);
	~CVkDevice();

	void PickPhysicalDevice(CVkInstance &instance);
	void CreateLogicalDevice();
	VkDeviceCreateInfo CreateDeviceCreateInfo(const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
                                              const VkPhysicalDeviceFeatures& deviceFeatures) const noexcept;
	bool IsDeviceSuitable(const VkPhysicalDevice& device) const;
	bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const;

	// Maybe should be "const VkPhysicalDevice & getPhysicalDevice() const;"
	// The same for "getLogicalDevice()"
	VkPhysicalDevice & getPhysicalDevice();
	VkDevice & getLogicalDevice();

private: 
	VkPhysicalDevice m_physicalDevice;
	VkDevice m_logicalDevice;
};
