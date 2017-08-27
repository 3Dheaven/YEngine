#pragma once

#include <stdlib.h>
#include <string>
#include <sstream>
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <windows.h>
#include <vulkan.h>
#include "CVulkanException.h"
#include "CVkCommon.hpp"

class CVkInstance
{
private:
	VkInstance mInstance;
	bool mVulkanInitialized;

public:
	CVkInstance();
	~CVkInstance();

	VkInstance& get();

	void createInstance(const VkInstanceCreateInfo& createInfo);
	void initializeVulkan(std::vector<const char*> requiredExtensions);
};