#pragma once
#include "wx/wxprec.h"
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <windows.h>
#include <vulkan.h>
#include "CVkCommon.hpp"
#include "CVkDevice.hpp"

/*
The swap chain handles the image buffers we will be writing to. 
It's essentially a queue of images that are waiting to be presented to the screen.
*/

class CVkSwapChain
{
private: 
	VkInstance mInstance;
	CVkDevice mDevice;
	VkPhysicalDevice mPhysicalDevice;
	
public:
	VkSurfaceKHR mSurface;
	VkSwapchainKHR mSwapChain = VK_NULL_HANDLE;
	std::vector<VkImage> m_swapchainImages;
	VkFormat m_swapchainImageFormat;
	VkExtent2D m_swapchainExtent;
	// Images are accessed through image views rather than directly.
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

	void connectDevice(CVkDevice &device)
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

		QueueFamilyIndices indices = mDevice.findQueueFamilies(mPhysicalDevice, mSurface);
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

	void CreateSwapChain(const wxSize& size)
	{
		// Checking for swap chain support from the device.
		SwapChainSupportDetails swapChainSupport = mDevice.querySwapChainSupport(mPhysicalDevice);
		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, size);
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}
		VkSwapchainCreateInfoKHR createInfo = CreateSwapchainCreateInfo(swapChainSupport,
			surfaceFormat, imageCount, extent);
		/*VkSwapchainKHR oldSwapchain = mSwapChain;
		createInfo.oldSwapchain = oldSwapchain;*/

		VkSwapchainKHR newSwapchain;
		VkResult result = vkCreateSwapchainKHR(mDevice.mLogicalDevice, &createInfo, nullptr, &newSwapchain);
		if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Error attempting to create a swapchain:");
		}
		*&mSwapChain = newSwapchain;

		result = vkGetSwapchainImagesKHR(mDevice.mLogicalDevice, mSwapChain, &imageCount, nullptr);
		if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Error attempting to retrieve the count of swapchain images:");
		}
		m_swapchainImages.resize(imageCount);
		result = vkGetSwapchainImagesKHR(mDevice.mLogicalDevice, mSwapChain, &imageCount, m_swapchainImages.data());
		if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Error attempting to retrieve the swapchain images:");
		}
		m_swapchainImageFormat = surfaceFormat.format;
		m_swapchainExtent = extent;

		// Create an image view for eacj image in the swapchain.
		m_swapchainImageViews.resize(m_swapchainImages.size());
		for (uint32_t i = 0; i < m_swapchainImages.size(); i++) 
		{
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_swapchainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_swapchainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			VkResult result = vkCreateImageView(mDevice.mLogicalDevice, &createInfo, nullptr, &m_swapchainImageViews[i]);
			if (result != VK_SUCCESS) 
			{
				throw CVulkanException(result, "Unable to create an image view for a swap chain image");
			}
		}
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

};
