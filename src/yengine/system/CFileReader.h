#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

namespace fs
{
	static std::vector<char> readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			std::stringstream ss;
			ss << "Failed to open file : " << filename;
			throw std::runtime_error(ss.str().c_str());
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}
}

