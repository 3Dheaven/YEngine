#include "StringsHelper.h"

namespace strh
{
	template<typename Out>
	void split(const std::string &s, char delim, Out result)
	{
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (std::getline(ss, item, delim))
		{
			if (!item.empty())
			{
				*(result++) = item;
			}
		}
	}

	std::vector<std::string> split(const std::string &s, char delim)
	{
		std::vector<std::string> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}

	void testSplitFunction()
	{
		std::vector<std::string> x = split("one:two::three", ':');
	}
}