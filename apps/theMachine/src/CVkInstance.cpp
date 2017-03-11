#include "CVkInstance.h"

CVkInstance::CVkInstance() :
	mVulkanInitialized(false),
	mInstance(VK_NULL_HANDLE)
{
	std::vector<const char*> requiredExtensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };

	initializeVulkan(requiredExtensions);

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "YEngineApp";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "YEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	std::vector<const char*> layerNames;
	if (enableValidationLayers)
	{
		layerNames = validationLayers;
	}

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = requiredExtensions.size();
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();
	createInfo.enabledLayerCount = layerNames.size();
	createInfo.ppEnabledLayerNames = layerNames.data();

	createInstance(createInfo);
};

CVkInstance::~CVkInstance()
{
}

VkInstance& 
CVkInstance::get()
{
	return mInstance;
}

void
CVkInstance::initializeVulkan(std::vector<const char*> requiredExtensions)
{
	// make sure that the Vulkan library is available on this system
#ifdef _WIN32
	HMODULE vulkanModule = ::LoadLibraryA("vulkan-1.dll");
	if (vulkanModule == NULL)
	{
		throw std::runtime_error("Vulkan library is not available on this system, so program cannot run.\n"
			"You must install the appropriate Vulkan library and also have a graphics card that supports Vulkan.");
	}
#else
#error Only Win32 is currently supported. To see how to support other windowing systems, \
	see the definition of _glfw_dlopen in XXX_platform.h and its use in vulkan.c in the glfw\
	source code. XXX specifies the windowing system (e.g. x11 for X11, and wl for Wayland).
#endif
	// make sure that the correct extensions are available
	uint32_t count;
	VkResult err = vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
	if (err != VK_SUCCESS)
	{
		throw CVulkanException(err, "Failed to retrieve the instance extension properties:");
	}
	std::vector<VkExtensionProperties> extensions(count);
	err = vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());
	if (err != VK_SUCCESS)
	{
		throw CVulkanException(err, "Failed to retrieve the instance extension properties:");
	}
	for (int extNum = 0; extNum < extensions.size(); ++extNum)
	{
		for (auto& iter = requiredExtensions.begin(); iter < requiredExtensions.end(); ++iter)
		{
			if (std::string(*iter) == extensions[extNum].extensionName)
			{
				requiredExtensions.erase(iter);
				break;
			}
		}
	};

	if (!requiredExtensions.empty())
	{
		std::stringstream ss;
		ss << "The following required Vulkan extensions could not be found:\n";
		for (int extNum = 0; extNum < requiredExtensions.size(); ++extNum)
		{
			ss << requiredExtensions[extNum] << "\n";
		}
		ss << "Program cannot continue.";
		throw std::runtime_error(ss.str());
	}

	mVulkanInitialized = true;
}

void
CVkInstance::createInstance(const VkInstanceCreateInfo& createInfo)
{
	if (!mVulkanInitialized)
	{
		throw std::runtime_error("Programming Error:\nAttempted to create a Vulkan instance before Vulkan was initialized.");
	}

	VkResult err = vkCreateInstance(&createInfo, nullptr, &mInstance);

	if (err != VK_SUCCESS)
	{
		throw CVulkanException(err, "Unable to create a Vulkan instance:");
	}
}

