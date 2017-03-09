#include "CVkDevice.hpp"

CVkDevice::CVkDevice(CVkInstance &instance):m_physicalDevice(VK_NULL_HANDLE), m_logicalDevice(VK_NULL_HANDLE)
{
	PickPhysicalDevice(instance);
	CreateLogicalDevice();
}

CVkDevice::~CVkDevice()
{
}

void CVkDevice::PickPhysicalDevice(CVkInstance &instance)
{
	auto &inst = instance.getInstance();
	if (!inst)
	{
		throw std::runtime_error("Programming Error:\n"
			"Attempted to get a Vulkan physical device before the Vulkan instance was created.");
	}

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(inst, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("Failed to find a GPU with Vulkan support.");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(inst, &deviceCount, devices.data());

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

bool CVkDevice::CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const
{
	uint32_t extensionCount;
	VkResult result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Cannot retrieve count of properties for a physical device:");
	}
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Cannot retrieve properties for a physical device:");
	}
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

VkPhysicalDevice & CVkDevice::getPhysicalDevice()
{
	return m_physicalDevice;
}

VkDevice & CVkDevice::getLogicalDevice()
{
	return m_logicalDevice;
}

VkDeviceCreateInfo CVkDevice::CreateDeviceCreateInfo(
	const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
	const VkPhysicalDeviceFeatures& deviceFeatures) const noexcept
{
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}
	return createInfo;
}

void CVkDevice::CreateLogicalDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(m_physicalDevice);
	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = CreateQueueCreateInfos(uniqueQueueFamilies);
	VkPhysicalDeviceFeatures deviceFeatures = {};
	VkDeviceCreateInfo createInfo = CreateDeviceCreateInfo(queueCreateInfos, deviceFeatures);

	VkResult result = vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Unable to create a logical device");
	}
	vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily, 0, &m_presentQueue);
}

bool CVkDevice::IsDeviceSuitable(const VkPhysicalDevice& device) const
{
	QueueFamilyIndices indices = FindQueueFamilies(device);
	bool extensionsSupported = CheckDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = mSwapChain.QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	return indices.IsComplete() & extensionsSupported && swapChainAdequate;
}