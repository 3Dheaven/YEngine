#pragma once
#include "wx/wxprec.h"
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <windows.h>
#include <vulkan.h>
#include "CVkCommon.hpp"

struct SwapChainSupportDetails 
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class CVkSwapChain
{
private: 
	VkInstance mInstance;
	VkDevice mDevice;
	VkPhysicalDevice mPhysicalDevice;
	
public:
	VkSurfaceKHR mSurface;
	VkSwapchainKHR mSwapChain = VK_NULL_HANDLE;
	std::vector<VkImage> m_swapchainImages;
	VkFormat m_swapchainImageFormat;
	VkExtent2D m_swapchainExtent;
	std::vector<VkImageView> m_swapchainImageViews;

	VkFormat colorFormat;
	VkColorSpaceKHR colorSpace;

	void connectInstance(VkInstance &instance)
	{
		this->mInstance = instance;
	}

	void connectSurface(VkSurfaceKHR &surface)
	{
		this->mSurface = surface;
	}

	void connectPhysicalDevice(VkPhysicalDevice &physicalDevice)
	{
		this->mPhysicalDevice = physicalDevice;
	}

	void connectDevice(VkDevice &device)
	{
		this->mDevice = device;
	}

/*#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR CreateWin32SurfaceCreateInfo(HWND *hwnd) const noexcept
	{
		VkWin32SurfaceCreateInfoKHR sci = {};
		sci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		sci.hwnd = *hwnd; //
		sci.hinstance = GetModuleHandle(NULL);
		return sci;
	}
#endif

	void CreateWindowSurface(HWND *hwnd)
	{
		if (!mInstance) 
		{
			throw std::runtime_error("Programming Error:\n"
				"Attempted to create a window surface before the Vulkan instance was created.");
		}
#ifdef _WIN32
		VkWin32SurfaceCreateInfoKHR sci = CreateWin32SurfaceCreateInfo(hwnd);
		VkResult err = vkCreateWin32SurfaceKHR(mInstance, &sci, nullptr, &mSurface);
		if (err != VK_SUCCESS) 
		{
			throw CVulkanException(err, "Cannot create a Win32 Vulkan surface:");
		}
#else
#error The code in CVulkanCanvas::CreateWindowSurface only supports Win32. Changes are \
required to support other windowing systems.
#endif
	}*/
	
	void CreateSwapChain(const wxSize& size)
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(mPhysicalDevice);
		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, size);
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}
		VkSwapchainCreateInfoKHR createInfo = CreateSwapchainCreateInfo(swapChainSupport,
			surfaceFormat, imageCount, extent);
		VkSwapchainKHR oldSwapchain = mSwapChain;
		createInfo.oldSwapchain = oldSwapchain;
		VkSwapchainKHR newSwapchain;
		VkResult result = vkCreateSwapchainKHR(mDevice, &createInfo, nullptr, &newSwapchain);
		if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Error attempting to create a swapchain:");
		}
		*&mSwapChain = newSwapchain;

		result = vkGetSwapchainImagesKHR(mDevice, mSwapChain, &imageCount, nullptr);
		if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Error attempting to retrieve the count of swapchain images:");
		}
		m_swapchainImages.resize(imageCount);
		result = vkGetSwapchainImagesKHR(mDevice, mSwapChain, &imageCount, m_swapchainImages.data());
		if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Error attempting to retrieve the swapchain images:");
		}
		m_swapchainImageFormat = surfaceFormat.format;
		m_swapchainExtent = extent;
	};


	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,
		const wxSize& size) const noexcept
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			VkExtent2D actualExtent = { static_cast<uint32_t>(size.GetWidth()),
				static_cast<uint32_t>(size.GetHeight()) };
			actualExtent.width = std::max(capabilities.minImageExtent.width,
				std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height,
				std::min(capabilities.maxImageExtent.height, actualExtent.height));
			return actualExtent;
		}
	};

	VkSwapchainCreateInfoKHR CreateSwapchainCreateInfo(
		const SwapChainSupportDetails& swapChainSupport,
		const VkSurfaceFormatKHR& surfaceFormat,
		uint32_t imageCount, const VkExtent2D& extent)
	{
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = mSurface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = FindQueueFamilies(mPhysicalDevice, mSurface);
		uint32_t queueFamilyIndices[] = { static_cast<uint32_t>(indices.graphicsFamily),
			static_cast<uint32_t>(indices.presentFamily) };
		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		return createInfo;
	};

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>& availableFormats) const noexcept
	{
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
			return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
				availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		return availableFormats[0];
	};

	VkPresentModeKHR ChooseSwapPresentMode(
		const std::vector<VkPresentModeKHR>& availablePresentModes) const noexcept
	{
		for (const auto& availablePresentMode : availablePresentModes) 
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
			{
				return availablePresentMode;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device) const
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


};