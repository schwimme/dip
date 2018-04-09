#pragma once

#include <atomic>


namespace cross
{

struct instance_counter
{
	std::atomic<uint32_t> m_cnt;
};


}
