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
#include "CVkFramebuffer.hpp"
#include "CVkInstance.h"
#include "CFileReader.h"
#include "../../../yengine/scene/CScene.h"

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
	
	CVkInstance mVulkanInstance;
	CVkSwapChain m_swapChain;
	CVkDevice m_device;
	CVkShader mShader;
	CVkFramebuffer mFramebuffers;

	void prepareVertices();
	void prepareUniformBuffers();

	void CreateVertexBuffer(VkBuffer &, VkDeviceMemory &);
	
	void CreateIndexBuffer(VkBuffer &, VkDeviceMemory &);
	VkDeviceMemory m_uniformMemorie, m_vertexMemory, m_indexMemory;
	std::vector<glm::vec2> m_vertices;
	std::vector<uint16_t> m_indices;
	VkBuffer m_uniformBuffer, m_vertexBuffer, m_indexBuffer;


	void CreateUniformBuffer(VkBuffer &, uint32_t size, VkDeviceMemory &deviceMemorie);

	
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






	void CreateWindowSurface(HWND *hwnd);
	void CreateGraphicsPipeline(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	void CreateCommandBuffers();

	/*Create two semaphores. The first one is used to signal that an image has been acquired and is ready for rendering, 
	and the second one to signal that rendering has finished and presentation can happen.*/
	void CreateSemaphores();

	void RecreateSwapchain();

	virtual void OnPaint(wxPaintEvent& event);
	virtual void OnResize(wxSizeEvent& event);
	virtual void onTimer(wxTimerEvent& event);
	void OnPaintException(const std::string& msg);

	
	VkSurfaceKHR m_surface;

	VkPipelineLayout m_pipelineLayout;
	VkPipeline m_graphicsPipeline;
	
	std::vector<VkCommandBuffer> m_commandBuffers;

	/* Specify synchronization objects that are to be signaled when the presentation engine is finished using the image.
	That's the point in time where we can start drawing to it.*/
	VkSemaphore m_imageAvailableSemaphore;
	// This semaphore is signaled once the command buffer(s) have finished execution. 
	VkSemaphore m_renderFinishedSemaphore;

	std::unique_ptr<wxTimer> m_timer;
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> sclock;
	sclock m_startTime;
	
};

