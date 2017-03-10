#include "CVkDevice.hpp"

CVkDevice::CVkDevice()
{
	m_logicalDevice = VK_NULL_HANDLE;
	m_physicalDevice = VK_NULL_HANDLE;
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
CVkDevice::PickPhysicalDevice()
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

	for (const auto& device : devices)
	{
		if (IsDeviceSuitable(device))
		{
			m_physicalDevice = device;
			break;
		}
	}

	if (m_physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("No physical GPU could be found with the required extensions and swap chain support.");
	}
}

bool 
CVkDevice::CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const
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

VkPhysicalDevice& 
CVkDevice::getPhysicalDevice()
{
	return m_physicalDevice;
}

VkDevice& 
CVkDevice::getLogicalDevice()
{
	return m_logicalDevice;
}

void 
CVkDevice::CreateLogicalDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(m_physicalDevice, mSurface);
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

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) 
	{
		createInfo.enabledLayerCount = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else 
	{
		createInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice);

	if (result != VK_SUCCESS) 
	{
		throw CVulkanException(result, "Unable to create a logical device");
	}

	vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily, 0, &m_presentQueue);
}

QueueFamilyIndices
CVkDevice::FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR &surface) const
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

bool 
CVkDevice::IsDeviceSuitable(const VkPhysicalDevice& device) const
{
	QueueFamilyIndices indices = FindQueueFamilies(device, mSurface);
	bool extensionsSupported = CheckDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	return indices.IsComplete() & extensionsSupported && swapChainAdequate;
}

SwapChainSupportDetails 
CVkDevice::QuerySwapChainSupport(const VkPhysicalDevice& device) const
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
