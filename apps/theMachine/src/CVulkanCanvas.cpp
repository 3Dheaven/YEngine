#include "CVulkanCanvas.h"
#include "CVulkanException.h"

#include <vulkan.h>
#include <fstream>
#include <sstream>

#pragma comment(lib, "vulkan-1.lib")

CVulkanCanvas::CVulkanCanvas(wxWindow *pParent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name)
	: wxWindow(pParent, id, pos, size, style, name),
	
	m_surface(VK_NULL_HANDLE),  m_pipelineLayout(VK_NULL_HANDLE),
	m_graphicsPipeline(VK_NULL_HANDLE), 
	m_imageAvailableSemaphore(VK_NULL_HANDLE), m_renderFinishedSemaphore(VK_NULL_HANDLE),
	mBuffer(m_device)
{
	Bind(wxEVT_PAINT, &CVulkanCanvas::OnPaint, this);
	Bind(wxEVT_SIZE, &CVulkanCanvas::OnResize, this);
	Bind(wxEVT_TIMER, &CVulkanCanvas::onTimer, this);

	// Initialize timer
	m_timer = std::make_unique<wxTimer>(this, 1000 / 60);
	m_timer->Start(3);
	m_startTime = std::chrono::high_resolution_clock::now();

	

	m_swapChain.connectInstance(mVulkanInstance.get());
	auto a = GetHwnd();
	
	CreateWindowSurface(&a);
	m_swapChain.connectSurface(m_surface);
	
	m_device.connectInstance(mVulkanInstance.get());
	m_device.connectSurface(m_surface);
	

	m_device.pickPhysicalDevice();
	m_swapChain.connectPhysicalDevice(m_device.mPhysicalDevice);

	m_device.createLogicalDevice();
	m_swapChain.connectDevice(m_device);

	m_device.connectSwapChain(m_swapChain.mSwapChain);
	
	mShader.ConnectDevice(m_device.mLogicalDevice);

	m_device.createCommandPool();

	m_pParent = pParent->GetParent();




	// Uniform buffer, allocation memory and binding
	CreateUniformBuffer(m_uniformBuffer, sizeof(glm::vec4), m_uniformMemorie);

	// Initialize the memory
	void* data;
	auto triangleColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	MapMemory(m_uniformMemorie, sizeof(glm::vec4), 0, &data);
	memcpy(data, &triangleColor, sizeof(glm::vec4));
	vkUnmapMemory(m_device.mLogicalDevice, m_uniformMemorie);

	// Descriptor in the shader
	CreateDescriptorSetLayout(1, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);

	// NEED A BIT OF REFACTORING...
	m_descriptorPoolSize.descriptorCount = 1;
	m_descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.maxSets = 1;
	descriptorPoolCreateInfo.pNext = nullptr;
	descriptorPoolCreateInfo.poolSizeCount = 1;
	descriptorPoolCreateInfo.pPoolSizes = &m_descriptorPoolSize;
	auto result = vkCreateDescriptorPool(m_device.mLogicalDevice, &descriptorPoolCreateInfo, nullptr, &m_descriptorPool);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to create a pool descriptor:");
	}

	m_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	m_descriptorSetAllocateInfo.pNext = nullptr;
	m_descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
	m_descriptorSetAllocateInfo.descriptorSetCount = 1;
	m_descriptorSetAllocateInfo.pSetLayouts = &m_descriptorSetLayout;

	result = vkAllocateDescriptorSets(m_device.mLogicalDevice, &m_descriptorSetAllocateInfo, &m_descriptorSet);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to allocate a descriptor set:");
	}

	bufferInfo.buffer = m_uniformBuffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(glm::vec4);

	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = m_descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;

	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;

	descriptorWrite.pBufferInfo = &bufferInfo;
	descriptorWrite.pImageInfo = nullptr; // Optional
	descriptorWrite.pTexelBufferView = nullptr; // Optional

	vkUpdateDescriptorSets(m_device.mLogicalDevice, 1, &descriptorWrite, 0, nullptr);



	

	// Rectangle vertices
	m_vertices.push_back(glm::vec2(-0.5f, -0.5f));
	m_vertices.push_back(glm::vec2(0.5f, -0.5f));
	m_vertices.push_back(glm::vec2(0.5f, 0.5f));
	m_vertices.push_back(glm::vec2(-0.5f, 0.5f));

	// Rectangle indices
	std::vector<uint16_t> indexRectangle = { 0, 1, 2, 2, 3, 0 };
	
	// Create vertex buffer, memory, bind buffer/memory and map memory
	CreateVertexBuffer(m_vertexBuffer, m_vertexMemory);

	// Create index buffer, memory, bind buffer/memory and map memory
	m_indices.insert(m_indices.begin(), indexRectangle.begin(), indexRectangle.end());
	CreateIndexBuffer(m_indexBuffer, m_indexMemory);

	mShader.mBindingDescription.binding = 0;
	mShader.mBindingDescription.stride = sizeof(glm::vec2);
	mShader.mBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	mShader.mAttributeDescriptions.emplace_back();
	auto &attributeDescription = mShader.mAttributeDescriptions.back();
	attributeDescription.binding = 0;
	attributeDescription.location = 0;
	attributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescription.offset = 0;


	m_swapChain.CreateSwapChain(size);

	mFramebuffers.connectSwapChain(m_swapChain);
	mFramebuffers.connectDevice(m_device);
	mFramebuffers.CreateRenderPass();
	CreateGraphicsPipeline("../workshop/vk_2d_square/vert.spv", "../workshop/vk_2d_square/frag.spv");

	mFramebuffers.CreateFrameBuffers();
	
	CreateCommandBuffers();
	CreateSemaphores();
}

CVulkanCanvas::~CVulkanCanvas() noexcept
{
	auto &logicalDevice = m_device.mLogicalDevice;
	m_timer->Stop();

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

			vkFreeMemory(logicalDevice, m_uniformMemorie, nullptr);
			vkDestroyBuffer(logicalDevice, m_uniformBuffer, nullptr);

			vkDestroyDevice(logicalDevice, nullptr);
		}
		vkDestroySurfaceKHR(mVulkanInstance.get(), m_surface, nullptr);
		vkDestroyInstance(mVulkanInstance.get(), nullptr);
	}
}

#ifdef _WIN32
VkWin32SurfaceCreateInfoKHR CVulkanCanvas::CreateWin32SurfaceCreateInfo(HWND *hwnd) const noexcept
{
	VkWin32SurfaceCreateInfoKHR sci = {};
	sci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	sci.hwnd = *hwnd;//GetHwnd();
	sci.hinstance = GetModuleHandle(NULL);
	return sci;
}
#endif

void CVulkanCanvas::CreateWindowSurface(HWND *hwnd)
{
	if (!mVulkanInstance.get()) {
		throw std::runtime_error("Programming Error:\n"
			"Attempted to create a window surface before the Vulkan instance was created.");
	}
#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR sci = CreateWin32SurfaceCreateInfo(hwnd);
	VkResult err = vkCreateWin32SurfaceKHR(mVulkanInstance.get(), &sci, nullptr, &m_surface);
	if (err != VK_SUCCESS) {
		throw CVulkanException(err, "Cannot create a Win32 Vulkan surface:");
	}
#else
#error The code in CVulkanCanvas::CreateWindowSurface only supports Win32. Changes are \
required to support other windowing systems.
#endif
}






VkViewport CVulkanCanvas::CreateViewport() const noexcept
{
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)m_swapChain.m_swapchainExtent.width;
	viewport.height = (float)m_swapChain.m_swapchainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	return viewport;
}

VkRect2D CVulkanCanvas::CreateScissor() const noexcept
{
	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = m_swapChain.m_swapchainExtent;
	return scissor;
}

VkPipelineViewportStateCreateInfo CVulkanCanvas::CreatePipelineViewportStateCreateInfo(
	const VkViewport& viewport, const VkRect2D& scissor) const noexcept
{
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;
	return viewportState;
}

VkPipelineRasterizationStateCreateInfo CVulkanCanvas::CreatePipelineRasterizationStateCreateInfo() const noexcept
{
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	return rasterizer;
}

VkPipelineMultisampleStateCreateInfo CVulkanCanvas::CreatePipelineMultisampleStateCreateInfo() const noexcept
{
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	return multisampling;
}

VkPipelineColorBlendAttachmentState CVulkanCanvas::CreatePipelineColorBlendAttachmentState() const noexcept
{
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	return colorBlendAttachment;
}

VkPipelineColorBlendStateCreateInfo CVulkanCanvas::CreatePipelineColorBlendStateCreateInfo(
	const VkPipelineColorBlendAttachmentState& colorBlendAttachment) const noexcept
{
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
	return colorBlending;
}

VkPipelineLayoutCreateInfo CVulkanCanvas::CreatePipelineLayoutCreateInfo() const noexcept
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &m_descriptorSetLayout;
	return pipelineLayoutInfo;
}

VkGraphicsPipelineCreateInfo CVulkanCanvas::CreateGraphicsPipelineCreateInfo(
	const VkPipelineShaderStageCreateInfo shaderStages[],
	const VkPipelineVertexInputStateCreateInfo& vertexInputInfo,
	const VkPipelineInputAssemblyStateCreateInfo& inputAssembly,
	const VkPipelineViewportStateCreateInfo& viewportState,
	const VkPipelineRasterizationStateCreateInfo& rasterizer,
	const VkPipelineMultisampleStateCreateInfo& multisampling,
	const VkPipelineColorBlendStateCreateInfo& colorBlending) const noexcept
{
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
	return pipelineInfo;
}

void CVulkanCanvas::CreateGraphicsPipeline(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
	auto vertShaderCode = ReadFile(vertexShaderFile);
	auto fragShaderCode = ReadFile(fragmentShaderFile);

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
	VkViewport viewport = CreateViewport();
	VkRect2D scissor = CreateScissor();
	VkPipelineViewportStateCreateInfo viewportState = CreatePipelineViewportStateCreateInfo(
		viewport, scissor);
	VkPipelineRasterizationStateCreateInfo rasterizer = CreatePipelineRasterizationStateCreateInfo();
	VkPipelineMultisampleStateCreateInfo multisampling = CreatePipelineMultisampleStateCreateInfo();
	VkPipelineColorBlendAttachmentState colorBlendAttachment = CreatePipelineColorBlendAttachmentState();
	VkPipelineColorBlendStateCreateInfo colorBlending = CreatePipelineColorBlendStateCreateInfo(
		colorBlendAttachment);
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = CreatePipelineLayoutCreateInfo();

	auto &logicalDevice = m_device.mLogicalDevice;

	VkResult result = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Failed to create pipeline layout:");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = CreateGraphicsPipelineCreateInfo(shaderStages,
		vertexInputInfo, inputAssembly, viewportState, rasterizer, multisampling, colorBlending);

	result = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline);
	// vkDestroyShaderModule calls below must be placed before possible throw of exception
	vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);
	vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Failed to create graphics pipeline:");
	}
}

std::vector<char> CVulkanCanvas::ReadFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		std::stringstream ss;
		ss << "Failed to open file: " << filename;
		throw std::runtime_error(ss.str().c_str());
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

VkCommandBufferAllocateInfo CVulkanCanvas::CreateCommandBufferAllocateInfo() const noexcept
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_device.mCommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();
	return allocInfo;
}

VkCommandBufferBeginInfo CVulkanCanvas::CreateCommandBufferBeginInfo() const noexcept
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	return beginInfo;
}

VkRenderPassBeginInfo CVulkanCanvas::CreateRenderPassBeginInfo(size_t swapchainBufferNumber) const noexcept
{
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = mFramebuffers.mRenderPass;
	renderPassInfo.framebuffer = mFramebuffers.mSwapchainFramebuffers[swapchainBufferNumber];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_swapChain.m_swapchainExtent;

	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;
	return renderPassInfo;
}

void CVulkanCanvas::CreateCommandBuffers()
{
	auto &logicalDevice = m_device.mLogicalDevice;

	if (m_commandBuffers.size() > 0)
	{
		vkFreeCommandBuffers(logicalDevice, m_device.mCommandPool, m_commandBuffers.size(), m_commandBuffers.data());
	}
	m_commandBuffers.resize(mFramebuffers.mSwapchainFramebuffers.size());

	VkCommandBufferAllocateInfo allocInfo = CreateCommandBufferAllocateInfo();
	VkResult result = vkAllocateCommandBuffers(logicalDevice, &allocInfo, m_commandBuffers.data());
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Failed to allocate command buffers:");
	}

	for (size_t i = 0; i < m_commandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo = CreateCommandBufferBeginInfo();
		result = vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo);

		if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Failed to begin command buffer:");
		}

		VkRenderPassBeginInfo renderPassInfo = CreateRenderPassBeginInfo(i);

		vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

		VkBuffer vertexBuffers[] = { m_vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(m_commandBuffers[i], m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

		vkCmdBindDescriptorSets(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSet, 0, nullptr);
		//vkCmdDraw(m_commandBuffers[i], m_vertices.size(), 1, 0, 0);
		vkCmdDrawIndexed(m_commandBuffers[i], m_indices.size(), 1, 0, 0, 0);
		vkCmdEndRenderPass(m_commandBuffers[i]);

		result = vkEndCommandBuffer(m_commandBuffers[i]);
		if (result != VK_SUCCESS)
		{
			throw CVulkanException(result, "Failed to record command buffer:");
		}
	}
}

VkSemaphoreCreateInfo CVulkanCanvas::CreateSemaphoreCreateInfo() const noexcept
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	return semaphoreInfo;
}

void CVulkanCanvas::CreateSemaphores()
{
	VkSemaphoreCreateInfo semaphoreInfo = CreateSemaphoreCreateInfo();

	auto &logicalDevice = m_device.mLogicalDevice;

	VkResult result = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Failed to create image available semaphore:");
	}
	result = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Failed to create render finished semaphore:");
	}
}

void CVulkanCanvas::RecreateSwapchain()
{
	vkDeviceWaitIdle(m_device.mLogicalDevice);

	wxSize size = GetSize();
	m_swapChain.CreateSwapChain(size);
	mFramebuffers.CreateRenderPass();
	CreateGraphicsPipeline("../workshop/vk_2d_square/vert.spv", "../workshop/vk_2d_square/frag.spv");
	mFramebuffers.CreateFrameBuffers();
	CreateCommandBuffers();
}

VkSubmitInfo CVulkanCanvas::CreateSubmitInfo(uint32_t imageIndex, VkPipelineStageFlags* waitStageFlags) const noexcept
{
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &m_imageAvailableSemaphore;
	submitInfo.pWaitDstStageMask = waitStageFlags;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &m_renderFinishedSemaphore;
	return submitInfo;
}

VkPresentInfoKHR CVulkanCanvas::CreatePresentInfoKHR(uint32_t& imageIndex) const noexcept
{
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &m_renderFinishedSemaphore;

	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &m_swapChain.mSwapChain;

	presentInfo.pImageIndices = &imageIndex;

	return presentInfo;
}

void CVulkanCanvas::onTimer(wxTimerEvent& event)
{
	ProcessEvent(wxPaintEvent());
}

void CVulkanCanvas::OnPaint(wxPaintEvent& event)
{
	auto &logicalDevice = m_device.mLogicalDevice;

	try {

		/*
		auto t_now = std::chrono::high_resolution_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::microseconds>(t_now - m_startTime).count();
		*/

		/*auto parent = (MainWindow *)m_pParent;
		if (parent->colorHasChanged)
		{
			glm::vec4 newColor;
			newColor.x = static_cast<float>(parent->color.Red()) / 255.0f;
			newColor.y = static_cast<float>(parent->color.Green()) / 255.0f;
			newColor.z = static_cast<float>(parent->color.Blue()) / 255.0f;
			newColor.w = 1.0f;
			*/
			void* data;
			glm::vec4 newColor = glm::vec4(1.0,0.0,0.0,1.0);
			MapMemory(m_uniformMemorie, sizeof(glm::vec4), 0, &data);
			memcpy(data, &newColor, sizeof(glm::vec4));
			vkUnmapMemory(logicalDevice, m_uniformMemorie);
			/*parent->colorHasChanged = false;
		}*/

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(logicalDevice, m_swapChain.mSwapChain,
			std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			RecreateSwapchain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw CVulkanException(result, "Failed to acquire swap chain image");
		}

		VkPipelineStageFlags waitFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSubmitInfo submitInfo = CreateSubmitInfo(imageIndex, waitFlags);

		// Submit command buffer to the queue
		result = vkQueueSubmit(m_device.mGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		if (result != VK_SUCCESS) {
			throw CVulkanException(result, "Failed to submit draw command buffer:");
		}

		VkPresentInfoKHR presentInfo = CreatePresentInfoKHR(imageIndex);
		result = vkQueuePresentKHR(m_device.mPresentQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
			RecreateSwapchain();
		}
		else if (result != VK_SUCCESS) {
			throw CVulkanException(result, "Failed to present swap chain image:");
		}
	}
	catch (const CVulkanException& ve) {
		std::string status = ve.GetStatus();
		std::stringstream ss;
		ss << ve.what() << "\n" << status;
		CallAfter(&CVulkanCanvas::OnPaintException, ss.str());
	}
	catch (const std::exception& err) {
		std::stringstream ss;
		ss << "Error encountered trying to create the Vulkan canvas:\n";
		ss << err.what();
		CallAfter(&CVulkanCanvas::OnPaintException, ss.str());
	}
}

void CVulkanCanvas::OnResize(wxSizeEvent& event)
{
	wxSize size = GetSize();
	if (size.GetWidth() == 0 || size.GetHeight() == 0) {
		return;
	}
	RecreateSwapchain();
	wxRect refreshRect(size);
	RefreshRect(refreshRect, false);
}

void CVulkanCanvas::OnPaintException(const std::string& msg)
{
	wxMessageBox(msg, "Vulkan Error");
	wxTheApp->ExitMainLoop();
}

void CVulkanCanvas::MapMemory(VkDeviceMemory memory, uint64_t size, uint64_t offset, void **data)
{
	auto result = vkMapMemory(m_device.mLogicalDevice, memory, offset, size, 0, data);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to map memory:");
	}
}

void CVulkanCanvas::AllocateMemory(VkDeviceMemory &deviceMemorie, VkBuffer &buffer, VkMemoryPropertyFlags properties)
{
	auto createInfo = CreateMemoryAllocateInfo(buffer, properties);

	auto result = vkAllocateMemory(m_device.mLogicalDevice, &createInfo, nullptr, &deviceMemorie);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to allocate memory:");
	}
}

uint32_t CVulkanCanvas::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_device.mPhysicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

VkMemoryAllocateInfo CVulkanCanvas::CreateMemoryAllocateInfo(VkBuffer &buffer, VkMemoryPropertyFlags properties)
{
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(m_device.mLogicalDevice, buffer, &memoryRequirements);

	VkMemoryAllocateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.allocationSize = memoryRequirements.size;
	createInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties);

	return createInfo;
}

void CVulkanCanvas::CreateUniformBuffer(VkBuffer &buffer, uint32_t size, VkDeviceMemory &deviceMemorie)
{
	CreateBuffer(buffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, size, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, deviceMemorie);
}

void CVulkanCanvas::CreateVertexBuffer(VkBuffer &buffer, VkDeviceMemory &deviceMemorie)
{
	VkDeviceSize bufferSize = sizeof(glm::vec2) * m_vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	CreateBuffer(stagingBuffer, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, bufferSize,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBufferMemory);

	void* data;
	MapMemory(stagingBufferMemory, bufferSize, 0, &data);
	memcpy(data, m_vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_device.mLogicalDevice, stagingBufferMemory);

	CreateBuffer(buffer, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, bufferSize,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, deviceMemorie);

	CopyBuffer(stagingBuffer, buffer, bufferSize);
}

void CVulkanCanvas::CreateIndexBuffer(VkBuffer &buffer, VkDeviceMemory &deviceMemorie)
{
	VkDeviceSize bufferSize = sizeof(uint16_t) * m_indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	CreateBuffer(stagingBuffer, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, bufferSize,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBufferMemory);

	void* data;
	MapMemory(stagingBufferMemory, bufferSize, 0, &data);
	memcpy(data, m_indices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_device.mLogicalDevice, stagingBufferMemory);

	CreateBuffer(buffer, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, bufferSize,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, deviceMemorie);

	CopyBuffer(stagingBuffer, buffer, bufferSize);
}

void CVulkanCanvas::CopyBuffer(const VkBuffer &srcBuffer, VkBuffer &dstBuffer, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_device.mCommandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	auto result = vkAllocateCommandBuffers(m_device.mLogicalDevice, &allocInfo, &commandBuffer);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to allocate command buffers:");
	}

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	// Start recording a command buffer
	result = vkBeginCommandBuffer(commandBuffer, &beginInfo);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to begin command buffer:");
	}

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	result = vkEndCommandBuffer(commandBuffer);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to end command buffer:");
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	result = vkQueueSubmit(m_device.mGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to submit queue:");
	}

	result = vkQueueWaitIdle(m_device.mGraphicsQueue);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to wait queue:");
	}

	vkFreeCommandBuffers(m_device.mLogicalDevice, m_device.mCommandPool, 1, &commandBuffer);
}

void CVulkanCanvas::CreateBuffer(VkBuffer &buffer,
	VkBufferUsageFlags usage,
	uint32_t size,
	VkMemoryPropertyFlags properties,
	VkDeviceMemory &deviceMemorie)
{

	auto createInfo = CreateBufferCreateInfo(size, usage);

	auto result = vkCreateBuffer(m_device.mLogicalDevice, &createInfo, nullptr, &buffer);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to create a buffer:");
	}

	AllocateMemory(deviceMemorie, buffer, properties);

	result = vkBindBufferMemory(m_device.mLogicalDevice, buffer, deviceMemorie, 0);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to bind buffer and memory:");
	}
}

VkBufferCreateInfo CVulkanCanvas::CreateBufferCreateInfo(uint64_t size,
	VkBufferUsageFlags usage)
{
	assert(size > 0);
	assert(usage != 0);

	VkBufferCreateInfo createInfo = {};

	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.size = size;
	createInfo.usage = usage;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	return createInfo;
}

void CVulkanCanvas::CreateDescriptorSetLayout(uint32_t bindingCount, uint32_t binding, VkDescriptorType descriptorType,
	uint32_t descriptorCount, uint32_t stageFlags)
{
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = CreateDescriptorSetLayoutInfo(
		bindingCount, binding, descriptorType, descriptorCount, stageFlags);

	auto result = vkCreateDescriptorSetLayout(m_device.mLogicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Failed to create descriptor set layout:");
	}
}

VkDescriptorSetLayoutCreateInfo CVulkanCanvas::CreateDescriptorSetLayoutInfo(uint32_t bindingCount,
	uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t stageFlags)
{
	m_descriptorSetLayoutBinding = CreateDescriptorSetLayoutBinding(binding, descriptorType, descriptorCount, stageFlags);

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;

	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.pNext = nullptr;
	descriptorSetLayoutCreateInfo.bindingCount = bindingCount;
	descriptorSetLayoutCreateInfo.pBindings = &m_descriptorSetLayoutBinding;

	return descriptorSetLayoutCreateInfo;
}

VkDescriptorSetLayoutBinding CVulkanCanvas::CreateDescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType,
	uint32_t descriptorCount, uint32_t stageFlags)
{
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding;

	descriptorSetLayoutBinding.binding = binding;
	descriptorSetLayoutBinding.descriptorType = descriptorType;
	descriptorSetLayoutBinding.descriptorCount = descriptorCount;
	descriptorSetLayoutBinding.stageFlags = stageFlags;
	descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

	return descriptorSetLayoutBinding;
}