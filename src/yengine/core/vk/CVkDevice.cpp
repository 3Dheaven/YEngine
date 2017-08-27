#include "CVkDevice.hpp"

CVkDevice::CVkDevice(): 
	mLogicalDevice(VK_NULL_HANDLE), 
	mPhysicalDevice(VK_NULL_HANDLE),
	mCommandPool(VK_NULL_HANDLE)
{
}

CVkDevice::~CVkDevice()
{
}

void 
CVkDevice::connectInstance(VkInstance &instance)
{
	this->mInstance = instance;
}

void 
CVkDevice::connectSwapChain(VkSwapchainKHR &swapChain)
{
	this->mSwapChain = swapChain;
}

void 
CVkDevice::connectSurface(VkSurfaceKHR &surface)
{
	this->mSurface = surface;
}

void 
CVkDevice::pickPhysicalDevice()
{
	if (!mInstance)
	{
		throw std::runtime_error("Programming Error:\n"
			"Attempted to get a Vulkan physical device before the Vulkan instance was created.");
	}

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("Failed to find a GPU with Vulkan support.");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data());

	// We need to iterate over all physical devices and find the one that supports rendering to our surface and has a graphics queue.
	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			mPhysicalDevice = device;
			break;
		}
	}

	if (mPhysicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("No physical GPU could be found with the required extensions and swap chain support.");
	}
}

bool 
CVkDevice::checkDeviceExtensionSupport(const VkPhysicalDevice& device) const
{
	uint32_t extensionCount;
	VkResult result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	if (result != VK_SUCCESS) 
	{
		throw CVulkanException(result, "Cannot retrieve count of properties for a physical device:");
	}
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
	if (result != VK_SUCCESS) 
	{
		throw CVulkanException(result, "Cannot retrieve properties for a physical device:");
	}
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto& extension : availableExtensions) 
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

void 
CVkDevice::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(mPhysicalDevice, mSurface);
	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	for (int queueFamily : uniqueQueueFamilies)
	{
		float queuePriority = 1.0f;
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.fillModeNonSolid = true;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) 
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else 
	{
		createInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mLogicalDevice);

	if (result != VK_SUCCESS) 
	{
		throw CVulkanException(result, "Unable to create a logical device");
	}

	vkGetDeviceQueue(mLogicalDevice, indices.graphicsFamily, 0, &mGraphicsQueue);
	vkGetDeviceQueue(mLogicalDevice, indices.graphicsFamily, 0, &mPresentQueue);
}

QueueFamilyIndices
CVkDevice::findQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR &surface) const
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
		if (indices.isComplete())
		{
			break;
		}
		++i;
	}
	return indices;
}

bool 
CVkDevice::isDeviceSuitable(const VkPhysicalDevice& device) const
{
	QueueFamilyIndices indices = findQueueFamilies(device, mSurface);
	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	return indices.isComplete() & extensionsSupported && swapChainAdequate;
}

SwapChainSupportDetails 
CVkDevice::querySwapChainSupport(const VkPhysicalDevice& device) const
{
	SwapChainSupportDetails details;

	VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface, &details.capabilities);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Unable to retrieve physical device surface capabilities:");
	}
	uint32_t formatCount = 0;
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, nullptr);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Unable to retrieve the number of formats for a surface on a physical device:");
	}
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, details.formats.data());
		if (result != VK_SUCCESS) {
			throw CVulkanException(result, "Unable to retrieve the formats for a surface on a physical device:");
		}
	}

	uint32_t presentModeCount = 0;
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, nullptr);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Unable to retrieve the count of present modes for a surface on a physical device:");
	}
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, details.presentModes.data());
		if (result != VK_SUCCESS) {
			throw CVulkanException(result, "Unable to retrieve the present modes for a surface on a physical device:");
		}
	}
	return details;
}

void CVkDevice::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(mPhysicalDevice, mSurface);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	poolInfo.pNext = nullptr;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

	VkResult result = vkCreateCommandPool(mLogicalDevice, &poolInfo, nullptr, &mCommandPool);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Failed to create command pool:");
	}
}


void CVkDevice::CopyBuffer(const VkBuffer &srcBuffer, VkBuffer &dstBuffer, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = mCommandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	auto result = vkAllocateCommandBuffers(mLogicalDevice, &allocInfo, &commandBuffer);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to allocate command buffers:");
	}

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	// Start recording a command buffer
	result = vkBeginCommandBuffer(commandBuffer, &beginInfo);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to begin command buffer:");
	}

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	result = vkEndCommandBuffer(commandBuffer);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to end command buffer:");
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	result = vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to submit queue:");
	}

	result = vkQueueWaitIdle(mGraphicsQueue);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to wait queue:");
	}

	vkFreeCommandBuffers(mLogicalDevice, mCommandPool, 1, &commandBuffer);
}

void CVkDevice::CreateBuffer(VkBuffer &buffer,
	VkBufferUsageFlags usage,
	uint32_t size,
	VkMemoryPropertyFlags properties,
	VkDeviceMemory &deviceMemorie)
{
	assert(size > 0);
	assert(usage != 0);

	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.size = size;
	createInfo.usage = usage;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	auto result = vkCreateBuffer(mLogicalDevice, &createInfo, nullptr, &buffer);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to create a buffer:");
	}

	AllocateMemory(deviceMemorie, buffer, properties);

	result = vkBindBufferMemory(mLogicalDevice, buffer, deviceMemorie, 0);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to bind buffer and memory:");
	}
}


void CVkDevice::AllocateMemory(VkDeviceMemory &deviceMemorie, VkBuffer &buffer, VkMemoryPropertyFlags properties)
{
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(mLogicalDevice, buffer, &memoryRequirements);

	VkMemoryAllocateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.allocationSize = memoryRequirements.size;
	createInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties);

	auto result = vkAllocateMemory(mLogicalDevice, &createInfo, nullptr, &deviceMemorie);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to allocate memory:");
	}
}

uint32_t CVkDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}


