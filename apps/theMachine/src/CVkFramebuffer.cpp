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

CVkFramebuffer::CVkFramebuffer()
{
	mRenderPass = VK_NULL_HANDLE;
}

CVkFramebuffer::~CVkFramebuffer()
{
	/*if (mRenderPass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(mDevice.mLogicalDevice, mRenderPass, nullptr);
	}*/
	//vkDestroyFramebuffer(mDevice.mLogicalDevice, framebuffer, nullptr);
}

void CVkFramebuffer::CreateRenderPass()
{
	VkAttachmentDescription colorAttachment = CreateAttachmentDescription();
	VkAttachmentReference colorAttachmentRef = CreateAttachmentReference();
	VkSubpassDescription subPass = CreateSubpassDescription(colorAttachmentRef);
	VkSubpassDependency dependency = CreateSubpassDependency();
	VkRenderPassCreateInfo renderPassInfo = CreateRenderPassCreateInfo(colorAttachment,
		subPass, dependency);

	VkResult result = vkCreateRenderPass(mDevice.mLogicalDevice, &renderPassInfo, nullptr, &mRenderPass);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Failed to create a render pass:");
	}
}


VkAttachmentDescription CVkFramebuffer::CreateAttachmentDescription() const noexcept
{
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = mSwapChain.m_swapchainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	return colorAttachment;
}

VkAttachmentReference CVkFramebuffer::CreateAttachmentReference() const noexcept
{
	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	return colorAttachmentRef;
}

VkSubpassDescription CVkFramebuffer::CreateSubpassDescription(
	const VkAttachmentReference& attachmentRef) const noexcept
{
	VkSubpassDescription subPass = {};
	subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subPass.colorAttachmentCount = 1;
	subPass.pColorAttachments = &attachmentRef;
	return subPass;
}

VkSubpassDependency CVkFramebuffer::CreateSubpassDependency() const noexcept
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

VkRenderPassCreateInfo 
CVkFramebuffer::CreateRenderPassCreateInfo(
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