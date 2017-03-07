#pragma once
#include <stdexcept>
#include <map>
#include <string>
#include <vulkan.h>
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

