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

class CVkFramebuffer
{
public:
	
	VkRenderPass m_renderPass;
	
	std::vector<VkFramebuffer> m_swapchainFramebuffers;

	CVkSwapChain mSwapChain;
	CVkDevice mDevice;

	void connectSwapChain(CVkSwapChain &swapChain)
	{
		this->mSwapChain = swapChain;
	}

	void connectDevice(CVkDevice &device)
	{
		this->mDevice = device;
	}

	void connectRenderpass(VkRenderPass &renderPass)
	{
		this->m_renderPass = renderPass;
	}

	void 
	CreateFrameBuffers()
	{
		VkFramebuffer framebuffer;
		m_swapchainFramebuffers.resize(mSwapChain.m_swapchainImageViews.size(), framebuffer);

		for (size_t i = 0; i < mSwapChain.m_swapchainImageViews.size(); i++)
		{
			VkImageView attachments[] =
			{
				mSwapChain.m_swapchainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = mSwapChain.m_swapchainExtent.width;
			framebufferInfo.height = mSwapChain.m_swapchainExtent.height;
			framebufferInfo.layers = 1;

			VK_CHECK_RESULT(vkCreateFramebuffer(mDevice.mLogicalDevice, &framebufferInfo, nullptr, &m_swapchainFramebuffers[i]),
							"Failed to create framebuffer:");
		}
	}
};
