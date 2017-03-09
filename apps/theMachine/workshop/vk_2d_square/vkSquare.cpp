#include "vkSquare.h"

vkSquare::vkSquare()
{
	mCam = NULL;
	mGDriver = NULL;
	mRightPanel = NULL;
}

vkSquare::vkSquare(CGraphicDriver *gdriver, wxPanel* panel)
{
	mCam = NULL;
	mGDriver = gdriver;
	mRightPanel = panel;
	loadGUI();
}

void 
vkSquare::init(CGraphicDriver *gdriver, wxPanel* panel)
{
	mGDriver = gdriver;
	mRightPanel = panel;
	setupGraphics();
	loadGUI();
}

vkSquare::~vkSquare()
{
	delete mCam;
	delete mScene;
	cleanGUI();
}

CCamera* 
vkSquare::getCam()
{
	return mCam;
}

void 
vkSquare::setupGraphics()
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

	std::string vsPath = projectPath + "//apps//theMachine//workshop//vk_2d_square//shader.vert";
	std::string fsPath = projectPath + "//apps//theMachine//workshop//vk_2d_square//shader.frag";

	mGDriver->createShader(vsPath, fsPath);

	mCam = new CCamera(glm::vec3(0.0f, 0.0f, 5.0f), 
						glm::vec3(0.0f, 0.0f, 0.0f), 
						glm::vec3(0.0f, 1.0f, 0.0f));

	mScene = new CScene(mGDriver);
	
	mUniformColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
	mGDriver->getShader()->setUniform("custom_color", mUniformColor);
	mUniformColorHasChanged = true;


	/*




	// Uniform buffer, allocation memory and binding
	CreateUniformBuffer(m_uniformBuffer, sizeof(glm::vec4), m_uniformMemorie);

	// Initialize the memory
	void* data;
	auto triangleColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	MapMemory(m_uniformMemorie, sizeof(glm::vec4), 0, &data);
	memcpy(data, &triangleColor, sizeof(glm::vec4));
	vkUnmapMemory(m_logicalDevice, m_uniformMemorie);

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
	auto result = vkCreateDescriptorPool(m_logicalDevice, &descriptorPoolCreateInfo, nullptr, &m_descriptorPool);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to create a pool descriptor:");
	}

	m_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	m_descriptorSetAllocateInfo.pNext = nullptr;
	m_descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
	m_descriptorSetAllocateInfo.descriptorSetCount = 1;
	m_descriptorSetAllocateInfo.pSetLayouts = &m_descriptorSetLayout;

	result = vkAllocateDescriptorSets(m_logicalDevice, &m_descriptorSetAllocateInfo, &m_descriptorSet);

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

	vkUpdateDescriptorSets(m_logicalDevice, 1, &descriptorWrite, 0, nullptr);



	CreateCommandPool();

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

	m_bindingDescription.binding = 0;
	m_bindingDescription.stride = sizeof(glm::vec2);
	m_bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	m_attributeDescriptions.emplace_back();
	auto &attributeDescription = m_attributeDescriptions.back();
	attributeDescription.binding = 0;
	attributeDescription.location = 0;
	attributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescription.offset = 0;




	CreateSwapChain(size);
	CreateImageViews();
	CreateRenderPass();
	CreateGraphicsPipeline("../shaders/vk/vert.spv", "../shaders/vk/frag.spv");
	CreateFrameBuffers();
	CreateCommandBuffers();
	CreateSemaphores();
	
	*/

}

void 
vkSquare::render()
{
	/*mGDriver->getShader()->use();

	mCam->projMatrix = glm::perspective(mCam->getZoom(), 1.0f, 0.1f, 100.0f);
	mGDriver->getShader()->setUniform("projection_matrix", mCam->projMatrix);
	mGDriver->getShader()->setUniform("view_matrix", mCam->getViewMatrix());
	// glm::vec3((sin(time * 1.0f) + 1.0f) / 2.0f, (sin(time * 0.5f) + 1.0f) / 2.0f, (cos(time * 0.25f) + 1.0f) / 2.0f)
	if (mUniformColorHasChanged)
	{
		mGDriver->getShader()->setUniform("custom_color", mUniformColor);
		mUniformColorHasChanged = false;
	}

	glm::mat4 model;
	//glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); 
	//model = glm::rotate(model, (float)time * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
	mGDriver->getShader()->setUniform("model_matrix", model);

	mScene->render(mGDriver->getShader());*/


	//try {

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
		/*void* data;
		glm::vec4 newColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
		MapMemory(m_uniformMemorie, sizeof(glm::vec4), 0, &data);
		memcpy(data, &newColor, sizeof(glm::vec4));
		vkUnmapMemory(m_logicalDevice, m_uniformMemorie);
		/*parent->colorHasChanged = false;
		}*/

		/*uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(m_logicalDevice, m_swapchain,
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

		result = vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		if (result != VK_SUCCESS) {
			throw CVulkanException(result, "Failed to submit draw command buffer:");
		}

		VkPresentInfoKHR presentInfo = CreatePresentInfoKHR(imageIndex);
		result = vkQueuePresentKHR(m_presentQueue, &presentInfo);
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
	}*/
}

void 
vkSquare::loadGUI()
{
	// http://zetcode.com/gui/wxwidgets/layoutmanagement/

	if (mRightPanel != NULL)
	{
		mRightPanel->DestroyChildren();
		mRightPanel->SetBackgroundColour(wxT("#ededed"));

		wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

		wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *st1 = new wxStaticText(mRightPanel, wxID_ANY, wxT("Color uniform : "));
		hbox1->Add(st1, 0, wxRIGHT, 10);
		wxColourPickerCtrl* colourPickerCtrl = new wxColourPickerCtrl(mRightPanel, wxID_ANY, wxStockGDI::COLOUR_RED);
		colourPickerCtrl->Bind(	wxEVT_COLOURPICKER_CHANGED, 
								[&](wxColourPickerEvent& evt) 
								{
									mUniformColorHasChanged = true;
									auto newColor = evt.GetColour();
									mUniformColor.x = static_cast<float>(newColor.Red()) / 255.0f;
									mUniformColor.y = static_cast<float>(newColor.Green()) / 255.0f;
									mUniformColor.z = static_cast<float>(newColor.Blue()) / 255.0f;
									mUniformColor.w = 1.0f;
								});
		hbox1->Add(colourPickerCtrl, 2);
		vbox->Add(hbox1, 0, wxLEFT | wxRIGHT | wxTOP, 10);

		vbox->Add(-1, 25); // bottom space

		mRightPanel->SetSizer(vbox);
		mRightPanel->Layout();
	}
}

void 
vkSquare::cleanGUI()
{
	if (mRightPanel != NULL)
	{
		mRightPanel->DestroyChildren();
	}
}

RendererRegister<vkSquare> regObjLoading("vkSquare");