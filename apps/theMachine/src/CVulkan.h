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

	struct sUniform
	{
		VkBuffer buffer;
		VkDeviceMemory memory;
		const char* name;
	};

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

	template<typename T> void
	updateUniform(T& value, const char* name)
	{
		bool uniformFound = false;
		for (auto &i : mUniforms)
		{
			if (!strcmp(i.name, name))
			{
				uniformFound = true;

				void* data;
				VK_CHECK_RESULT(vkMapMemory(m_device.mLogicalDevice, i.memory, 0, sizeof(T), 0, &data), 
								"Error attempting to map memory for uniform:");
				memcpy(data, &value, sizeof(T));
				vkUnmapMemory(m_device.mLogicalDevice, i.memory);
				break;
			}
		}
	}

	void prepareVertices(std::vector<sVertex>& vertices, std::vector<unsigned int>& indices);

	template<typename T> void
	prepareUniformBuffer(T& value, const char* name)
	{
		sUniform u;
		u.name = name;
		mUniforms.push_back(u);
		CreateUniformBuffer(mUniforms.back().buffer, sizeof(T), mUniforms.back().memory);

		void* data;
		VK_CHECK_RESULT(vkMapMemory(m_device.mLogicalDevice, mUniforms.back().memory, 0, sizeof(T), 0, &data),
						"Error attempting to map memory while preparing uniform:");

		memcpy(data, &value, sizeof(T));
		vkUnmapMemory(m_device.mLogicalDevice, u.memory);

		// Descriptor in the shader
		VkDescriptorSetLayoutBinding m_descriptorSetLayoutBinding;
		m_descriptorSetLayoutBinding.binding = 0;
		m_descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		m_descriptorSetLayoutBinding.descriptorCount = 1;
		m_descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS; // Access this set binding in all shader stages
		m_descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.pNext = nullptr;
		descriptorSetLayoutCreateInfo.bindingCount = 1;
		descriptorSetLayoutCreateInfo.pBindings = &m_descriptorSetLayoutBinding;

		VK_CHECK_RESULT(vkCreateDescriptorSetLayout(m_device.mLogicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout), 
						"Failed to create descriptor set layout:");

		VkDescriptorPoolSize m_descriptorPoolSize;
		m_descriptorPoolSize.descriptorCount = 1;
		m_descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.maxSets = 1;
		descriptorPoolCreateInfo.pNext = nullptr;
		descriptorPoolCreateInfo.poolSizeCount = 1;
		descriptorPoolCreateInfo.pPoolSizes = &m_descriptorPoolSize;

		VkDescriptorPool m_descriptorPool;
		VK_CHECK_RESULT(vkCreateDescriptorPool(m_device.mLogicalDevice, &descriptorPoolCreateInfo, nullptr, &m_descriptorPool),
						"Error attempting to create a pool descriptor:");

		VkDescriptorSetAllocateInfo m_descriptorSetAllocateInfo;
		m_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		m_descriptorSetAllocateInfo.pNext = nullptr;
		m_descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
		m_descriptorSetAllocateInfo.descriptorSetCount = 1;
		m_descriptorSetAllocateInfo.pSetLayouts = &m_descriptorSetLayout;

		VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device.mLogicalDevice, &m_descriptorSetAllocateInfo, &m_descriptorSet),
						"Error attempting to allocate a descriptor set:");

		VkDescriptorBufferInfo bufferInfo;
		bufferInfo.buffer = mUniforms.back().buffer;
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(T);

		VkWriteDescriptorSet descriptorWrite;
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


	}
	void finalizeSetup();
};

