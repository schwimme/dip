#pragma once


#include <vector>
#include <algorithm>
#include <iterator>


namespace sys
{
	
	
template<typename to_search_t, typename container_t>
bool find(typename container_t::const_iterator& it, const container_t& container, const to_search_t& toSearch)
{
	it = std::find(container.begin(), container.end(), toSearch);
	return it != container.end();
}


template<typename to_search_t, typename container_t>
bool find(const container_t& container, const to_search_t& toSearch)
{
	typename container_t::const_iterator it;
	return find(it, container, toSearch);
}


template<typename container_t>
container_t make_union(const container_t& first, const container_t& second)
{
	container_t tmp;
	std::set_union(
		first.begin(), first.end(),
		second.begin(), second.end(),
		std::back_inserter(tmp));

	return tmp;
}


}
