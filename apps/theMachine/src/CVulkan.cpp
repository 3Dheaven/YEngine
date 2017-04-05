#include "CVulkan.h"



CVulkan::CVulkan():
	m_surface(VK_NULL_HANDLE), 
	m_pipelineLayout(VK_NULL_HANDLE),
	m_graphicsPipeline(VK_NULL_HANDLE),
	m_imageAvailableSemaphore(VK_NULL_HANDLE), 
	m_renderFinishedSemaphore(VK_NULL_HANDLE)
{

}

void
CVulkan::prepare(HWND *hwnd, const wxSize& size)
{
	mSize = size;

	// Create windows surface
	CreateWindowSurface(hwnd);

	// Pick physical device
	m_device.connectInstance(mVulkanInstance.get());
	m_device.connectSurface(m_surface);
	m_device.pickPhysicalDevice();

	// Create device
	m_device.createLogicalDevice();

	m_swapChain.connectPhysicalDevice(m_device.mPhysicalDevice);
	m_swapChain.connectInstance(mVulkanInstance.get());
	m_swapChain.connectSurface(m_surface);
	m_swapChain.connectDevice(m_device);

	m_device.connectSwapChain(m_swapChain.mSwapChain);

	mShader.ConnectDevice(m_device.mLogicalDevice);

	m_device.createCommandPool();

	m_swapChain.CreateSwapChain(size);

	mFramebuffers.connectSwapChain(m_swapChain);
	mFramebuffers.connectDevice(m_device);
	mFramebuffers.CreateRenderPass();
	mFramebuffers.CreateFrameBuffers();
}

void CVulkan::finalizeSetup()
{
	std::vector<std::string> splittedPath = strh::split(sys::getExecutablePath(), '\\');
	std::string projectPath;
	for (auto i : splittedPath)
	{
		if (i != "YEngine")
		{
			projectPath += i + "\\";
		}
		else
		{
			projectPath += i;
			break;
		}
	}
	
	std::string vertfragShaderPath = "//apps//theMachine//workshop//cube//";
	std::string vertShaderFile = "shader.vert";
	std::string fragShaderFile = "shader.frag";

	std::string cmdVert = projectPath + "//tools//glslangValidator.exe -V " + 
					      projectPath + vertfragShaderPath + vertShaderFile + " -o " +
					      projectPath + vertfragShaderPath + "vert.spv";

	std::string cmdFrag = projectPath + "//tools//glslangValidator.exe -V " +
						  projectPath + vertfragShaderPath + fragShaderFile + " -o " +
						  projectPath + vertfragShaderPath + "frag.spv";

	std::cout << cmd::execute(cmdVert.c_str()) << std::endl;
	std::cout << cmd::execute(cmdFrag.c_str()) << std::endl;

	setupDescriptorSetLayout();

	setupDescriptorPool();

	setupDescriptorSet();

	CreateGraphicsPipeline(projectPath + vertfragShaderPath + "vert.spv",
						   projectPath + vertfragShaderPath + "frag.spv");

	CreateCommandBuffers();
	CreateSemaphores();
}

CVulkan::~CVulkan()
{
	auto &logicalDevice = m_device.mLogicalDevice;
	if (mVulkanInstance.get() != VK_NULL_HANDLE) {
		if (logicalDevice != VK_NULL_HANDLE) {
			vkDeviceWaitIdle(logicalDevice);
			if (m_graphicsPipeline != VK_NULL_HANDLE) {
				vkDestroyPipeline(logicalDevice, m_graphicsPipeline, nullptr);
			}
			if (m_pipelineLayout != VK_NULL_HANDLE) {
				vkDestroyPipelineLayout(logicalDevice, m_pipelineLayout, nullptr);
			}

			if (mFramebuffers.mRenderPass != VK_NULL_HANDLE)
			{
				vkDestroyRenderPass(logicalDevice, mFramebuffers.mRenderPass, nullptr);
			}

			if (m_swapChain.mSwapChain != VK_NULL_HANDLE) {
				vkDestroySwapchainKHR(logicalDevice, m_swapChain.mSwapChain, nullptr);
			}
			for (auto& imageView : m_swapChain.m_swapchainImageViews) {
				vkDestroyImageView(logicalDevice, imageView, nullptr);
			}
			for (auto& framebuffer : mFramebuffers.mSwapchainFramebuffers) {
				vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
			}
			if (m_device.mCommandPool != VK_NULL_HANDLE) {
				vkDestroyCommandPool(logicalDevice, m_device.mCommandPool, nullptr);
			}
			if (m_imageAvailableSemaphore != VK_NULL_HANDLE) {
				vkDestroySemaphore(logicalDevice, m_imageAvailableSemaphore, nullptr);
			}
			if (m_renderFinishedSemaphore != VK_NULL_HANDLE) {
				vkDestroySemaphore(logicalDevice, m_renderFinishedSemaphore, nullptr);
			}

			for (int i = 0; i < mUniforms.size(); i++)
			{
				vkFreeMemory(logicalDevice, mUniforms[i].memory, nullptr);
				vkDestroyBuffer(logicalDevice, mUniforms[i].buffer, nullptr);
			}

			vkDestroyDevice(logicalDevice, nullptr);
		}
		vkDestroySurfaceKHR(mVulkanInstance.get(), m_surface, nullptr);
		vkDestroyInstance(mVulkanInstance.get(), nullptr);
	}
}

void CVulkan::CreateWindowSurface(HWND *hwnd)
{
	if (!mVulkanInstance.get())
	{
		throw std::runtime_error("Programming Error:\n"
			"Attempted to create a window surface before the Vulkan instance was created.");
	}

#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR sci = {};
	sci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	sci.hwnd = *hwnd;
	sci.hinstance = GetModuleHandle(NULL);

	VkResult err = vkCreateWin32SurfaceKHR(mVulkanInstance.get(), &sci, nullptr, &m_surface);
	if (err != VK_SUCCESS)
	{
		throw CVulkanException(err, "Cannot create a Win32 Vulkan surface:");
	}
#else
#error The code in CVulkanCanvas::CreateWindowSurface only supports Win32. Changes are \
	required to support other windowing systems.
#endif
}

void CVulkan::CreateGraphicsPipeline(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
	auto vertShaderCode = fs::readFile(vertexShaderFile);
	auto fragShaderCode = fs::readFile(fragmentShaderFile);

	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;

	mShader.CreateShaderModule(vertShaderCode, vertShaderModule);
	mShader.CreateShaderModule(fragShaderCode, fragShaderModule);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = mShader.CreatePipelineShaderStageCreateInfo(
		VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule, "main");
	VkPipelineShaderStageCreateInfo fragShaderStageInfo = mShader.CreatePipelineShaderStageCreateInfo(
		VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule, "main");
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = mShader.CreatePipelineVertexInputStateCreateInfo();
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = mShader.CreatePipelineInputAssemblyStateCreateInfo(
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

	// Create viewport
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)m_swapChain.m_swapchainExtent.width;
	viewport.height = (float)m_swapChain.m_swapchainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	// CreateScissor
	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = m_swapChain.m_swapchainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_LINE;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &mDescriptorSetLayout;

	auto &logicalDevice = m_device.mLogicalDevice;

	VkResult result = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);
	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Failed to create pipeline layout:");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = m_pipelineLayout;
	pipelineInfo.renderPass = mFramebuffers.mRenderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	result = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline);
	// vkDestroyShaderModule calls below must be placed before possible throw of exception
	vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);
	vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);
	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Failed to create graphics pipeline:");
	}
}

void CVulkan::CreateCommandBuffers()
{
	auto &logicalDevice = m_device.mLogicalDevice;

	//----------------------------------------------------------------------------------------------------------
	// Command buffer allocation
	//----------------------------------------------------------------------------------------------------------

	// Cleaning up command buffers
	if (m_commandBuffers.size() > 0)
	{
		vkFreeCommandBuffers(logicalDevice, m_device.mCommandPool, m_commandBuffers.size(), m_commandBuffers.data());
	}
	m_commandBuffers.resize(mFramebuffers.mSwapchainFramebuffers.size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_device.mCommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;						// Can be submitted to a queue for execution, but cannot be called from other command buffers.
	allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

	VkResult result = vkAllocateCommandBuffers(logicalDevice, &allocInfo, m_commandBuffers.data());
	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Failed to allocate command buffers:");
	}

	for (size_t i = 0; i < m_commandBuffers.size(); i++)
	{
		//----------------------------------------------------------------------------------------------------------
		// Starting command buffer recording
		//----------------------------------------------------------------------------------------------------------

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;		/* The command buffer can be resubmitted while it is also already pending execution.
																			We have used the last flag because we may already be scheduling the drawing commands
																			for the next frame while the last frame is not finished yet.*/
		beginInfo.pInheritanceInfo = nullptr;								// Optional

		result = vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo);

		// barrier for reading from uniform buffer after all writing is done:
		VkMemoryBarrier uniformMemoryBarrier = {};
		uniformMemoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		uniformMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
		uniformMemoryBarrier.dstAccessMask = VK_ACCESS_UNIFORM_READ_BIT;

		vkCmdPipelineBarrier(m_commandBuffers[i],
			VK_PIPELINE_STAGE_HOST_BIT,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			0,
			1, &uniformMemoryBarrier,
			0, NULL,
			0, NULL);


		if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Failed to begin command buffer:");
		}

		//----------------------------------------------------------------------------------------------------------
		// Starting a render pass
		//----------------------------------------------------------------------------------------------------------

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = mFramebuffers.mRenderPass;
		renderPassInfo.framebuffer = mFramebuffers.mSwapchainFramebuffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_swapChain.m_swapchainExtent;
		renderPassInfo.clearValueCount = 1;
		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		// We can now bind the graphics pipeline
		vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

		VkBuffer vertexBuffers[] = { m_vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(m_commandBuffers[i], m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &mDescriptorSet, 0, nullptr);
		//vkCmdDraw(m_commandBuffers[i], m_vertices.size(), 1, 0, 0);
		vkCmdDrawIndexed(m_commandBuffers[i], m_indices.size(), 1, 0, 0, 0);
		auto s = m_indices.size();
		vkCmdEndRenderPass(m_commandBuffers[i]);

		// We've finished recording the command buffer
		result = vkEndCommandBuffer(m_commandBuffers[i]);
		if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Failed to record command buffer:");
		}
	}
}

void CVulkan::CreateSemaphores()
{
	auto &logicalDevice = m_device.mLogicalDevice;

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	// Semaphore to signal that an image has been acquired and is ready for rendering.
	VkResult result = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore);
	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Failed to create image available semaphore:");
	}

	// Semaphore to signal that rendering has finished and presentation can happen.
	result = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore);
	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Failed to create render finished semaphore:");
	}
}

void CVulkan::recreateSwapchain()
{
	vkDeviceWaitIdle(m_device.mLogicalDevice);

	m_swapChain.CreateSwapChain(mSize);
	mFramebuffers.CreateRenderPass();
	CreateGraphicsPipeline("../workshop/vk_2d_square/vert.spv", "../workshop/vk_2d_square/frag.spv");
	mFramebuffers.CreateFrameBuffers();
	CreateCommandBuffers();
}

void CVulkan::render()
{
	try
	{
		// ------------------------------------------------------------------------------------------------------------
		// The first thing to do is to acquiring an image from the swap chain
		// The swap chain is an extension feature, so we must use a function with the vk*KHR naming convention.
		// ------------------------------------------------------------------------------------------------------------

		/* Index of the swap chain image that has become available
		The index refers to the VkImage in our swapChainImages array.
		We're going to use that index to pick the right command buffer.*/
		uint32_t imageIndex;

		/*The first two parameters of vkAcquireNextImageKHR are the logical device and the swap chain from which we wish
		to acquire an image. The third parameter specifies a timeout in nanoseconds for an image to become available.
		Using the maximum value of a 64 bit unsigned integer disables the timeout.*/
		VkResult result = vkAcquireNextImageKHR(m_device.mLogicalDevice, m_swapChain.mSwapChain,
			std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapchain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw CVulkanException(result, "Failed to acquire swap chain image");
		}

		// ------------------------------------------------------------------------------------------------------------
		// Submit the command buffer
		// ------------------------------------------------------------------------------------------------------------

		// We want to wait with writing colors to the image until it's available, 
		// so we're specifying the stage of the graphics pipeline that writes to the color attachment. 
		VkPipelineStageFlags waitFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_imageAvailableSemaphore;	// Specify which semaphores to wait on before execution begins 
		submitInfo.pWaitDstStageMask = waitFlags;					// Specify in which stage(s) of the pipeline to wait.
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];	// Specify which command buffers to actually submit for execution.
																	// We should submit the command buffer that binds the swap chain image we just acquired as color attachment.
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_renderFinishedSemaphore;  // Specify which semaphores to signal once the command buffer(s) have finished execution. 

																	// We can now submit the command buffer to the graphics queue
		result = vkQueueSubmit(m_device.mGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Failed to submit draw command buffer:");
		}

		// ------------------------------------------------------------------------------------------------------------
		// Presentation
		// The last step of drawing a frame is submitting the result back to the swap chain to have it eventually show up on the screen.
		// ------------------------------------------------------------------------------------------------------------

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_renderFinishedSemaphore;	// Specify which semaphores to wait on before presentation can happen
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_swapChain.mSwapChain;			// Specify the swap chains to present images to
		presentInfo.pImageIndices = &imageIndex;					// Specify the index of the image for each swap chain
		presentInfo.pResults = nullptr;								// Optional. It allows to specify an array of VkResult values to check 
																	// for every individual swap chain if presentation was successful

																	// Submits the request to present an image to the swap chain. 
		result = vkQueuePresentKHR(m_device.mPresentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			recreateSwapchain();
		}
		else if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Failed to present swap chain image:");
		}
	}
	catch (const CVulkanException& ve)
	{
		std::string status = ve.GetStatus();
		std::stringstream ss;
		ss << ve.what() << "\n" << status;
		//CallAfter(&CVulkanCanvas::OnPaintException, ss.str());
	}
	catch (const std::exception& err)
	{
		std::stringstream ss;
		ss << "Error encountered trying to create the Vulkan canvas:\n";
		ss << err.what();
		//CallAfter(&CVulkanCanvas::OnPaintException, ss.str());
	}
}

// Prepare vertex and index buffers for an indexed triangle
void CVulkan::prepareVertices(std::vector<sVertex>& vertices, std::vector<unsigned int>& indices)
{
	auto v = vertices.size();
	auto isize = indices.size();

	for (auto i : vertices)
	{
		auto v = i.position;
		m_vertices.push_back(v);
	}

	// Create vertex buffer, memory, bind buffer/memory and map memory
	CreateVertexBuffer(m_vertexBuffer, m_vertexMemory);

	// Create index buffer, memory, bind buffer/memory and map memory
	m_indices.insert(m_indices.begin(), indices.begin(), indices.end());
	CreateIndexBuffer(m_indexBuffer, m_indexMemory);

	// We are now describing the Input Vertex Data
	// A vertex input binding is used to describe the data arrangement to the GPU.
	mShader.mBindingDescription.binding = 0;
	mShader.mBindingDescription.stride = sizeof(glm::vec3);					// Size of one vertex
	mShader.mBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	mShader.mAttributeDescriptions.emplace_back();
	auto &attributeDescription = mShader.mAttributeDescriptions.back();
	attributeDescription.binding = 0;										
	attributeDescription.location = 0;
	attributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescription.offset = 0;

}



void CVulkan::CreateUniformBuffer(VkBuffer &buffer, uint32_t size, VkDeviceMemory &deviceMemorie)
{
	m_device.CreateBuffer(buffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, size, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, deviceMemorie);
}

void CVulkan::CreateVertexBuffer(VkBuffer &buffer, VkDeviceMemory &deviceMemorie)
{
	VkDeviceSize bufferSize = sizeof(glm::vec3) * m_vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	m_device.CreateBuffer(stagingBuffer, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, bufferSize,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBufferMemory);

	void* data;

	auto result = vkMapMemory(m_device.mLogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to map memory:");
	}

	memcpy(data, m_vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_device.mLogicalDevice, stagingBufferMemory);

	m_device.CreateBuffer(buffer, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, bufferSize,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, deviceMemorie);

	m_device.CopyBuffer(stagingBuffer, buffer, bufferSize);
}

void CVulkan::CreateIndexBuffer(VkBuffer &buffer, VkDeviceMemory &deviceMemorie)
{
	VkDeviceSize bufferSize = sizeof(unsigned int) * m_indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	m_device.CreateBuffer(stagingBuffer, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, bufferSize,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBufferMemory);

	void* data;

	VK_CHECK_RESULT(vkMapMemory(m_device.mLogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data),
		"Error attempting to map memory:");

	memcpy(data, m_indices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_device.mLogicalDevice, stagingBufferMemory);

	m_device.CreateBuffer(buffer, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, bufferSize,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, deviceMemorie);

	m_device.CopyBuffer(stagingBuffer, buffer, bufferSize);
}

void 
CVulkan::setupDescriptorSetLayout()
{
	unsigned char numBinding = 0;
	std::vector<VkDescriptorSetLayoutBinding> mDescriptorSetLayoutBindings;

	for (auto &u : mUniforms)
	{
		VkDescriptorSetLayoutBinding m_descriptorSetLayoutBinding;
		m_descriptorSetLayoutBinding.binding = numBinding;
		m_descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		m_descriptorSetLayoutBinding.descriptorCount = 1;
		m_descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS; // Access to this binding in all shader stages
		m_descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		mDescriptorSetLayoutBindings.push_back(m_descriptorSetLayoutBinding);

		numBinding++;
	}

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.pNext = nullptr;
	descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(mDescriptorSetLayoutBindings.size()); // Number of VkDescriptorSetLayoutBinding
	descriptorSetLayoutCreateInfo.pBindings = mDescriptorSetLayoutBindings.data();
	descriptorSetLayoutCreateInfo.flags = 0;

	VK_CHECK_RESULT(vkCreateDescriptorSetLayout(m_device.mLogicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &mDescriptorSetLayout),
		"Failed to create descriptor set layout:");
}

void 
CVulkan::setupDescriptorPool()
{
	std::vector<VkDescriptorPoolSize> descriptorPoolSize;
	for (auto &u : mUniforms)
	{
		VkDescriptorPoolSize m_descriptorPoolSize;
		m_descriptorPoolSize.descriptorCount = 1;
		m_descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorPoolSize.push_back(m_descriptorPoolSize);
	}

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.maxSets = 1;
	descriptorPoolCreateInfo.pNext = nullptr;
	descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSize.size());
	descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSize.data();

	VK_CHECK_RESULT(vkCreateDescriptorPool(m_device.mLogicalDevice, &descriptorPoolCreateInfo, nullptr, &mDescriptorPool),
		"Error attempting to create a pool descriptor:");

}

void
CVulkan::setupDescriptorSet()
{
	VkDescriptorSetAllocateInfo m_descriptorSetAllocateInfo;
	m_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	m_descriptorSetAllocateInfo.pNext = nullptr;
	m_descriptorSetAllocateInfo.descriptorPool = mDescriptorPool;
	m_descriptorSetAllocateInfo.descriptorSetCount = 1;
	m_descriptorSetAllocateInfo.pSetLayouts = &mDescriptorSetLayout;

	VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device.mLogicalDevice, &m_descriptorSetAllocateInfo, &mDescriptorSet),
		"Error attempting to allocate a descriptor set:");

	std::vector<VkWriteDescriptorSet> mWriteDescriptorSets;
	for (auto &u : mUniforms)
	{
		VkWriteDescriptorSet descriptorWrite;
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = mDescriptorSet;
		descriptorWrite.dstBinding = mWriteDescriptorSets.size();
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &u.bufferInfos;
		descriptorWrite.pImageInfo = nullptr;								// Optional
		descriptorWrite.pTexelBufferView = nullptr;						    // Optional

		mWriteDescriptorSets.push_back(descriptorWrite);
	}

	vkUpdateDescriptorSets(m_device.mLogicalDevice, static_cast<uint32_t>(mWriteDescriptorSets.size()), mWriteDescriptorSets.data(), 0, NULL);
}