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

class CVulkan
{
public:
	CVulkan();
    ~CVulkan();

};

