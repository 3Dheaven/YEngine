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
#include <stdexcept>
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
#include "../../../yengine/system/CmdHandling.h"
#include "../../../yengine/scene/CMesh.h"

class CVulkan
{
private:

	struct sUniform
	{
		sUniform(const char* uniformName)
		{
			name = uniformName;
		};

		VkBuffer buffer;
		VkDeviceMemory memory;
		const char* name;
		VkDescriptorBufferInfo bufferInfos;
	};

	std::string vsShaderFullFilePath;
	std::string fsShaderFullFilePath;

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

	std::vector<sUniform> mUniforms;

	
	/*std::vector<*/VkDeviceMemory/*> ,*/ m_vertexMemory, m_indexMemory;
	/*std::vector<*/std::vector<glm::vec3>/*>*/ m_vertices;
	/*std::vector<*/std::vector<unsigned int>/*>*/ m_indices;
	VkBuffer m_vertexBuffer, m_indexBuffer;

	VkDescriptorSet mDescriptorSet;
	VkDescriptorSetLayout mDescriptorSetLayout;
	VkDescriptorPool mDescriptorPool;

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

	template<typename T> void
	updateUniform(T& value, const char* name)
	{
		for (auto &u : mUniforms)
		{
			if (!strcmp(u.name, name))
			{
				void* data;
				VK_CHECK_RESULT(vkMapMemory(m_device.mLogicalDevice, u.memory, 0, sizeof(T), 0, &data), 
								"Error attempting to map memory for uniform:");
				memcpy(data, &value, sizeof(T));

				// flush device memory
				VkMappedMemoryRange memoryRange = {};
				memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
				memoryRange.memory = u.memory;
				memoryRange.offset = 0;
				memoryRange.size = VK_WHOLE_SIZE;
				vkFlushMappedMemoryRanges(m_device.mLogicalDevice, 1, &memoryRange);
				
				vkUnmapMemory(m_device.mLogicalDevice, u.memory);
				break;
			}
		}
	}

	void prepareVertices(std::vector<sVertex>& vertices, std::vector<unsigned int>& indices);

	template<typename T> void
	prepareUniformBuffer(T& value, const char* name)
	{
		mUniforms.push_back(sUniform(name));
		sUniform *u = &mUniforms.back();
		void* data;

		CreateUniformBuffer(u->buffer, sizeof(T), u->memory);
				
		VK_CHECK_RESULT(vkMapMemory(m_device.mLogicalDevice, u->memory, 0, sizeof(T), 0, &data),
						"Error attempting to map memory while preparing uniform:");

		memcpy(data, &value, sizeof(T));

		vkUnmapMemory(m_device.mLogicalDevice, u->memory);

		u->bufferInfos.buffer = u->buffer;
		u->bufferInfos.offset = 0;
		u->bufferInfos.range = VK_WHOLE_SIZE;
	}

	/*
		Setup descriptors of each shader resource : type, binding location, from which stage the uniform will be available
		Those descriptors define the layout of a final descriptor set.
	*/
	void setupDescriptorSetLayout();

	/*
		Descriptor set contains storage for descriptors
	*/
	void setupDescriptorSet();

	void setupDescriptorPool();

	void finalizeSetup();

	void setupShader(const std::string vs, const std::string fs);
};

