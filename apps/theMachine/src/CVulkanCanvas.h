#pragma once

#include "wx/wxprec.h"
#include <wx/timer.h>
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan.h>
#include <string>
#include <set>
#include <memory>
#include <chrono>
#include "glm.hpp"
#include <wx/clrpicker.h>
#include "CRenderer.h"
#include "MainWindow.h"
#include "CVkSwapChain.hpp"
#include "CVkDevice.hpp"
#include "CVkCommon.hpp"
#include "CVkShader.hpp"
#include "CVkBuffer.hpp"

class CVulkanCanvas :
    public wxWindow
{
public:
	CVulkanCanvas(wxWindow *pParent,
        wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = "VulkanCanvasName");

    virtual ~CVulkanCanvas() noexcept;
	wxWindow *m_pParent;

	//void setGModule(CRenderer *renderer);
	//CRenderer*mGModule;

private:
	
	CVkSwapChain m_swapChain;
	CVkDevice m_device;
	CVkShader mShader;
	CVkBuffer mBuffer;

	void CreateVertexBuffer(VkBuffer &, VkDeviceMemory &);
	void CopyBuffer(const VkBuffer &, VkBuffer &, VkDeviceSize);
	void CreateIndexBuffer(VkBuffer &, VkDeviceMemory &);
	VkDeviceMemory m_uniformMemorie, m_vertexMemory, m_indexMemory;
	std::vector<glm::vec2> m_vertices;
	std::vector<uint16_t> m_indices;
	VkBuffer m_uniformBuffer, m_vertexBuffer, m_indexBuffer;

	void CreateBuffer(VkBuffer &, VkBufferUsageFlags, uint32_t size, VkMemoryPropertyFlags properties, VkDeviceMemory &deviceMemorie);
	void CreateUniformBuffer(VkBuffer &, uint32_t size, VkDeviceMemory &deviceMemorie);
	VkBufferCreateInfo CreateBufferCreateInfo(uint64_t, VkBufferUsageFlags);
	void AllocateMemory(VkDeviceMemory &deviceMemorie, VkBuffer &buffer, VkMemoryPropertyFlags properties);
	VkMemoryAllocateInfo CreateMemoryAllocateInfo(VkBuffer &buffer, VkMemoryPropertyFlags properties);
	void MapMemory(VkDeviceMemory, uint64_t, uint64_t, void **);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void CreateFrameBuffers();
	std::vector<VkFramebuffer> m_swapchainFramebuffers;
	VkFramebufferCreateInfo CreateFramebufferCreateInfo(const VkImageView& attachments) const noexcept;

	void InitializeVulkan(std::vector<const char*> extensions);
	void CreateInstance(const VkInstanceCreateInfo& createInfo);
	void CreateWindowSurface(HWND *hwnd);

	void CreateImageViews();
	void CreateRenderPass();
	void CreateGraphicsPipeline(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	
	void CreateCommandPool();
	void CreateCommandBuffers();
	void CreateSemaphores();
	void RecreateSwapchain();
	VkWin32SurfaceCreateInfoKHR CreateWin32SurfaceCreateInfo(HWND *hwnd) const noexcept;

	VkApplicationInfo CreateApplicationInfo(const std::string& appName,
		const int32_t appVersion = VK_MAKE_VERSION(1, 0, 0),
		const std::string& engineName = "YEngine",
		const int32_t engineVersion = VK_MAKE_VERSION(1, 0, 0),
		const int32_t apiVersion = VK_API_VERSION_1_0) const noexcept;
	VkInstanceCreateInfo CreateInstanceCreateInfo(const VkApplicationInfo& appInfo,
		const std::vector<const char*>& extensionNames,
		const std::vector<const char*>& layerNames) const noexcept;

	VkImageViewCreateInfo CreateImageViewCreateInfo(uint32_t swapchainImage) const noexcept;
	VkAttachmentDescription CreateAttachmentDescription() const noexcept;
	VkAttachmentReference CreateAttachmentReference() const noexcept;
	VkSubpassDescription CreateSubpassDescription(const VkAttachmentReference& attachmentRef) const noexcept;
	VkSubpassDependency CreateSubpassDependency() const noexcept;
	VkRenderPassCreateInfo CreateRenderPassCreateInfo(
		const VkAttachmentDescription& colorAttachment,
		const VkSubpassDescription& subPass,
		const VkSubpassDependency& dependency) const noexcept;
	VkViewport CreateViewport() const noexcept;
	VkRect2D CreateScissor() const noexcept;
	VkPipelineViewportStateCreateInfo CreatePipelineViewportStateCreateInfo(
		const VkViewport& viewport, const VkRect2D& scissor) const noexcept;
	VkPipelineRasterizationStateCreateInfo CreatePipelineRasterizationStateCreateInfo() const noexcept;
	VkPipelineMultisampleStateCreateInfo CreatePipelineMultisampleStateCreateInfo() const noexcept;
	VkPipelineColorBlendAttachmentState CreatePipelineColorBlendAttachmentState() const noexcept;
	VkPipelineColorBlendStateCreateInfo CreatePipelineColorBlendStateCreateInfo(
		const VkPipelineColorBlendAttachmentState& colorBlendAttachment) const noexcept;
	VkPipelineLayoutCreateInfo CreatePipelineLayoutCreateInfo() const noexcept;
	VkGraphicsPipelineCreateInfo CreateGraphicsPipelineCreateInfo(
		const VkPipelineShaderStageCreateInfo shaderStages[],
		const VkPipelineVertexInputStateCreateInfo& vertexInputInfo,
		const VkPipelineInputAssemblyStateCreateInfo& inputAssembly,
		const VkPipelineViewportStateCreateInfo& viewportState,
		const VkPipelineRasterizationStateCreateInfo& rasterizer,
		const VkPipelineMultisampleStateCreateInfo& multisampling,
		const VkPipelineColorBlendStateCreateInfo& colorBlending) const noexcept;
	
	VkCommandPoolCreateInfo CreateCommandPoolCreateInfo(QueueFamilyIndices& queueFamilyIndices) const noexcept;
	VkCommandBufferAllocateInfo CreateCommandBufferAllocateInfo() const noexcept;
	VkCommandBufferBeginInfo CreateCommandBufferBeginInfo() const noexcept;
	VkRenderPassBeginInfo CreateRenderPassBeginInfo(size_t swapchainBufferNumber) const noexcept;
	VkSemaphoreCreateInfo CreateSemaphoreCreateInfo() const noexcept;
	VkSubmitInfo CreateSubmitInfo(uint32_t imageIndex, VkPipelineStageFlags* pipelineStageFlags) const noexcept;
	VkPresentInfoKHR CreatePresentInfoKHR(uint32_t& imageIndex) const noexcept;
	
	static std::vector<char> ReadFile(const std::string& filename);
	
	virtual void OnPaint(wxPaintEvent& event);
	virtual void OnResize(wxSizeEvent& event);
	virtual void onTimer(wxTimerEvent& event);
	void OnPaintException(const std::string& msg);

	VkDescriptorSetLayoutBinding CreateDescriptorSetLayoutBinding(uint32_t, VkDescriptorType, uint32_t, uint32_t);
	void CreateDescriptorSetLayout(uint32_t, uint32_t, VkDescriptorType, uint32_t, uint32_t);
	VkDescriptorSetLayoutCreateInfo CreateDescriptorSetLayoutInfo(uint32_t, uint32_t, VkDescriptorType, uint32_t, uint32_t);

	VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	VkDescriptorSet m_descriptorSet;
	VkDescriptorSetLayout m_descriptorSetLayout;
	VkDescriptorPoolSize m_descriptorPoolSize;


	VkDescriptorSetLayoutBinding m_descriptorSetLayoutBinding;
	VkDescriptorPool m_descriptorPool;
	std::vector<VkPushConstantRange> m_ranges;
	VkDescriptorSetAllocateInfo m_descriptorSetAllocateInfo;
	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;

	VkDescriptorBufferInfo bufferInfo;
	VkWriteDescriptorSet descriptorWrite;

	VkInstance m_instance;
	VkSurfaceKHR m_surface;

	VkRenderPass m_renderPass;
	VkPipelineLayout m_pipelineLayout;
	VkPipeline m_graphicsPipeline;

	VkCommandPool m_commandPool;
	std::vector<VkCommandBuffer> m_commandBuffers;
	VkSemaphore m_imageAvailableSemaphore;
	VkSemaphore m_renderFinishedSemaphore;
	bool m_vulkanInitialized;
	std::unique_ptr<wxTimer> m_timer;
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> sclock;
	sclock m_startTime;
	
};

