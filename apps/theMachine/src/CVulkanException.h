#pragma once
#include <stdexcept>
#include <map>
#include <string>
#include <vulkan.h>

#define VK_CHECK_RESULT(f, m)  																		\
{																									\
	VkResult res = (f);																				\
	if (res != VK_SUCCESS)																			\
	{																								\
		throw CVulkanException(res, m);																\
	}																								\
}

class CVulkanException :
    public std::runtime_error
{
public:
	CVulkanException(const VkResult status, const std::string& msg);
    virtual ~CVulkanException();
    const std::string GetStatus() const;

private:
    const VkResult m_status;
    static std::map<VkResult, std::string> m_statuses;
};

