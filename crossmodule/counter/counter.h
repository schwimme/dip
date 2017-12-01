#pragma once

#include <atomic>


namespace crossmodule
{

struct instance_counter
{
	std::atomic<uint32_t> m_cnt;
};


}
