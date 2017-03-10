#include "CVkBuffer.hpp"

CVkBuffer::CVkBuffer(CVkDevice &device):mDevice(device)
{
}

CVkBuffer::~CVkBuffer()
{
}

void CVkBuffer::CreateBuffer(VkBufferUsageFlags usage,
	uint32_t size,
	VkMemoryPropertyFlags properties)
{

	auto createInfo = CreateBufferCreateInfo(size, usage);

	auto result = vkCreateBuffer(mDevice.mLogicalDevice, &createInfo, nullptr, &mBuffer);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to create a buffer:");
	}

	AllocateMemory(properties);

	result = vkBindBufferMemory(mDevice.mLogicalDevice, mBuffer, mDeviceMemory, 0);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to bind buffer and memory:");
	}
}

void CVkBuffer::CreateUniformBuffer(uint32_t size)
{
	CreateBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, size, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}

VkBufferCreateInfo CVkBuffer::CreateBufferCreateInfo(uint64_t size,
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

void CVkBuffer::AllocateMemory(VkMemoryPropertyFlags properties)
{
	auto createInfo = CreateMemoryAllocateInfo(properties);

	auto result = vkAllocateMemory(mDevice.mLogicalDevice, &createInfo, nullptr, &mDeviceMemory);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to allocate memory:");
	}
}

VkMemoryAllocateInfo CVkBuffer::CreateMemoryAllocateInfo(VkMemoryPropertyFlags properties)
{
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(mDevice.mLogicalDevice, mBuffer, &memoryRequirements);

	VkMemoryAllocateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.allocationSize = memoryRequirements.size;
	createInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties);

	return createInfo;
}

void CVkBuffer::MapMemory(uint64_t size, uint64_t offset, void **data)
{
	auto result = vkMapMemory(mDevice.mLogicalDevice, mDeviceMemory, offset, size, 0, data);

	if (result != VK_SUCCESS)
	{
		throw CVulkanException(result, "Error attempting to map memory:");
	}
}

uint32_t CVkBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(mDevice.mPhysicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}