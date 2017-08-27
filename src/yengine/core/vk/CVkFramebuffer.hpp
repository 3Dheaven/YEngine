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
#include "CVkSwapChain.hpp"
#include "CVkDevice.hpp"

struct CVkFramebuffer
{
	VkRenderPass mRenderPass;
	std::vector<VkFramebuffer> mSwapchainFramebuffers;
	CVkSwapChain mSwapChain;
	CVkDevice mDevice;

	CVkFramebuffer();
	~CVkFramebuffer();

	inline void CVkFramebuffer::connectSwapChain(CVkSwapChain &swapChain)
	{
		this->mSwapChain = swapChain;
	}

	inline void connectDevice(CVkDevice &device)
	{
		this->mDevice = device;
	}

	inline void connectRenderpass(VkRenderPass &renderPass)
	{
		this->mRenderPass = renderPass;
	}

	void CreateFrameBuffers();
	void CreateRenderPass();
	VkRenderPassCreateInfo CreateRenderPassCreateInfo(
		const VkAttachmentDescription& colorAttachment,
		const VkSubpassDescription& subPass,
		const VkSubpassDependency& dependency) const noexcept;
	VkSubpassDependency CreateSubpassDependency() const noexcept;

	VkSubpassDescription CreateSubpassDescription(
		const VkAttachmentReference& attachmentRef) const noexcept;

	VkAttachmentReference CreateAttachmentReference() const noexcept;
	VkAttachmentDescription CreateAttachmentDescription() const noexcept;
};
