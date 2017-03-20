#pragma once

#include "wx/wxprec.h"
#include <wx/timer.h>
#include <wx/clrpicker.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan.h>
#pragma comment(lib, "vulkan-1.lib")
#include <string>
#include <set>
#include <memory>
#include "glm.hpp"
#include "CVulkanException.h"
#include "CFileReader.h"
#include "CVkSwapChain.hpp"
#include "CVkDevice.hpp"
#include "CVkCommon.hpp"
#include "CVkShader.hpp"
#include "CVkFramebuffer.hpp"
#include "CVkInstance.h"
#include "../../../yengine/helpers/StringsHelper.h"
#include "../../../yengine/system/FileSystem.h"
#include "../../../yengine/scene/CMesh.h"

class CVulkan
{
private:

	CVkInstance mVulkanInstance;
	CVkSwapChain m_swapChain;
	CVkDevice m_device;
	CVkShader mShader;
	CVkFramebuffer mFramebuffers;
	VkSurfaceKHR m_surface;
	VkPipelineLayout m_pipelineLayout;
	VkPipeline m_graphicsPipeline;
	std::vector<VkCommandBuffer> m_commandBuffers;

	// Specify synchronization objects that are to be signaled when the presentation engine is finished using the image.
	// That's the point in time where we can start drawing to it.
	VkSemaphore m_imageAvailableSemaphore;
	// This semaphore is signaled once the command buffer(s) have finished execution. 
	VkSemaphore m_renderFinishedSemaphore;

	VkDeviceMemory m_uniformMemorie, m_vertexMemory, m_indexMemory;
	std::vector<glm::vec3> m_vertices;
	std::vector<unsigned int> m_indices;
	VkBuffer m_uniformBuffer, m_vertexBuffer, m_indexBuffer;
	VkDescriptorSet m_descriptorSet;
	VkDescriptorSetLayout m_descriptorSetLayout;

	wxSize mSize;

	void CreateVertexBuffer(VkBuffer &, VkDeviceMemory &);
	void CreateIndexBuffer(VkBuffer &, VkDeviceMemory &);
	void CreateUniformBuffer(VkBuffer &, uint32_t size, VkDeviceMemory &deviceMemorie);

	void CreateWindowSurface(HWND *hwnd);
	void CreateGraphicsPipeline(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	void CreateCommandBuffers();
	
	// Create two semaphores. The first one is used to signal that an image has been acquired and is ready for rendering,
	// and the second one to signal that rendering has finished and presentation can happen.
	void CreateSemaphores();

public:

	CVulkan();
    ~CVulkan();
	void prepare(HWND *hwnd, const wxSize& size);
	void render();
	void recreateSwapchain();
	void updateUniforms(glm::vec4 &value);
	void prepareVertices(std::vector<sVertex>& vertices, std::vector<unsigned int>& indices);
	void prepareUniformBuffers(glm::vec4& value);
	void finalizeSetup();
};

