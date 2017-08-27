#ifndef CFILESYSTEM_H
#define CFILESYSTEM_H

#include <string>
#include <sstream>
#include <vector>
#include <iterator>

namespace strh
{
	std::vector<std::string>
	split(const std::string &s, char delim);
}

#endif