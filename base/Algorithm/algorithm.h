#pragma once


#include <vector>
#include <algorithm>
#include <iterator>

namespace Base
{
	
	
template<typename ToSearchType, typename ContainerType>
bool Find(typename ContainerType::const_iterator& it, const ContainerType& container, const ToSearchType& toSearch)
{
	it = std::find(container.begin(), container.end(), toSearch);
	return it != container.end();
}


template<typename ToSearchType, typename ContainerType>
bool Find(const ContainerType& container, const ToSearchType& toSearch)
{
	ContainerType::const_iterator it;
	return Find(it, container, toSearch);
}


template<typename ContainerType>
ContainerType Union(const ContainerType& first, const ContainerType& second)
{
	ContainerType tmp;
	std::set_union(
		first.begin(), first.end(),
		second.begin(), second.end(),
		std::back_inserter(tmp));

	return tmp;
}


}