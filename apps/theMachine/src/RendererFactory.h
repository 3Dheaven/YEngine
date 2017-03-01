#pragma once

#include <iostream>
#include <map>
#include <utility>
#include <functional>
#include "CRenderer.h"
class CRenderer;
template<typename T> CRenderer* createT() 
{ 
	return new T; 
}

typedef std::map<std::string, CRenderer*(*)()> map_type;

struct RendererFactory 
{
	static CRenderer* createInstance(std::string const& s) 
	{
		map_type::iterator it = getMap()->find(s);
		if (it == getMap()->end())
			return 0;
		return it->second();
	}

	static map_type* getMap() 
	{
		if (!map) 
		{ 
			map = new map_type; 
		}
		return map;
	}

private:
	static map_type* map;
};

template<typename T>
struct RendererRegister : RendererFactory
{
	RendererRegister()
	{
		
	};

	RendererRegister(std::string const& s)
	{
		std::cout << "Registering a new renderer : " << s << std::endl;
		getMap()->insert(std::make_pair(s, &createT<T>));
	}
};
