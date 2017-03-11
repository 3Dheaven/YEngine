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
#include "CVkSwapChain.hpp"
#include "CVkDevice.hpp"

struct CVkFramebuffer
{
	VkRenderPass mRenderPass;
	std::vector<VkFramebuffer> mSwapchainFramebuffers;
	CVkSwapChain mSwapChain;
	CVkDevice mDevice;

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
};
