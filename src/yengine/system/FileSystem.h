#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <string.h>
#include <vector>
#include "../helpers/StringsHelper.h"

namespace sys
{
	std::string getFullFilePath();
	std::string getProjectPath(std::string projectRoot);
}
