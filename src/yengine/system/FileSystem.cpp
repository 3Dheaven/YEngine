#include "FileSystem.h"

namespace sys
{
	std::string
	getFullFilePath()
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::string::size_type pos = std::string(buffer).find_last_of("\\/");
		return std::string(buffer).substr(0, pos);
	}

	std::string
	getProjectPath(std::string projectRoot)
	{
		std::vector<std::string> splittedPath = strh::split(sys::getFullFilePath(), '\\');
		std::string projectPath;

		for (auto i : splittedPath)
		{
			if (i != projectRoot)
			{
				projectPath += i + "\\";
			}
			else
			{
				projectPath += i;
				break;
			}
		}

		return projectPath;
	}
}

