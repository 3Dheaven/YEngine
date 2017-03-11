#include "CVkFramebuffer.hpp"

void CVkFramebuffer::CreateFrameBuffers()
{
	VkFramebuffer framebuffer;
	mSwapchainFramebuffers.resize(mSwapChain.m_swapchainImageViews.size(), framebuffer);

	for (size_t i = 0; i < mSwapChain.m_swapchainImageViews.size(); i++)
	{
		VkImageView attachments[] =
		{
			mSwapChain.m_swapchainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = mRenderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = mSwapChain.m_swapchainExtent.width;
		framebufferInfo.height = mSwapChain.m_swapchainExtent.height;
		framebufferInfo.layers = 1;

		VK_CHECK_RESULT(vkCreateFramebuffer(mDevice.mLogicalDevice, &framebufferInfo, nullptr, &mSwapchainFramebuffers[i]),
			"Failed to create framebuffer:");
	}
}
