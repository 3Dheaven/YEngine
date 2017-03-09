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
private: 
	VkDevice m_logicalDevice;
	VkRenderPass m_renderPass;
	VkFormat m_swapchainImageFormat;
	std::vector<VkFramebuffer> m_swapchainFramebuffers;
	std::vector<VkImageView> m_swapchainImageViews;
	VkExtent2D m_swapchainExtent;

public:

	void CreateFrameBuffers()
	{
		VkFramebuffer framebuffer;
		m_swapchainFramebuffers.resize(m_swapchainImageViews.size(), framebuffer);

		for (size_t i = 0; i < m_swapchainImageViews.size(); i++)
		{
			VkImageView attachments[] =
			{
				m_swapchainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo = CreateFramebufferCreateInfo(*attachments);

			VkResult result = vkCreateFramebuffer(m_logicalDevice, &framebufferInfo, nullptr, &m_swapchainFramebuffers[i]);
			if (result != VK_SUCCESS)
			{
				throw CVulkanException(result, "Failed to create framebuffer:");
			}
		}
	}

	void CreateRenderPass()
	{
		VkAttachmentDescription colorAttachment = CreateAttachmentDescription();
		VkAttachmentReference colorAttachmentRef = CreateAttachmentReference();
		VkSubpassDescription subPass = CreateSubpassDescription(colorAttachmentRef);
		VkSubpassDependency dependency = CreateSubpassDependency();
		VkRenderPassCreateInfo renderPassInfo = CreateRenderPassCreateInfo(colorAttachment, subPass, dependency);

		VkResult result = vkCreateRenderPass(m_logicalDevice, &renderPassInfo, nullptr, &m_renderPass);
		if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Failed to create a render pass:");
		}
	}

	VkFramebufferCreateInfo CreateFramebufferCreateInfo(
		const VkImageView& attachments) const noexcept
	{
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = &attachments;
		framebufferInfo.width = m_swapchainExtent.width;
		framebufferInfo.height = m_swapchainExtent.height;
		framebufferInfo.layers = 1;
		return framebufferInfo;
	}

	VkAttachmentDescription CreateAttachmentDescription() const noexcept
	{
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = m_swapchainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		return colorAttachment;
	}

	VkAttachmentReference CreateAttachmentReference() const noexcept
	{
		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		return colorAttachmentRef;
	}

	VkSubpassDescription CreateSubpassDescription(
		const VkAttachmentReference& attachmentRef) const noexcept
	{
		VkSubpassDescription subPass = {};
		subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subPass.colorAttachmentCount = 1;
		subPass.pColorAttachments = &attachmentRef;
		return subPass;
	}

	VkSubpassDependency CreateSubpassDependency() const noexcept
	{
		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		return dependency;
	}

	VkRenderPassCreateInfo CreateRenderPassCreateInfo(
		const VkAttachmentDescription& colorAttachment,
		const VkSubpassDescription& subPass,
		const VkSubpassDependency& dependency) const noexcept
	{
		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subPass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;
		return renderPassInfo;
	}
};
