#pragma once

#include "wx/wxprec.h"
#include <stdlib.h>
#include <string>
#include <fstream>
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <windows.h>
#include <vulkan.h>
#include "CVulkanException.h"

class CVkInstance
{
public:
	CVkInstance();
	~CVkInstance();

	VkInstance & getInstance();

private:
	VkInstance m_instance;
};