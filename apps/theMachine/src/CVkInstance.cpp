#include "CVkInstance.h"

CVkInstance::CVkInstance()
{
}

CVkInstance::~CVkInstance()
{
}

VkInstance & CVkInstance::getInstance()
{
	return m_instance;
}

