#pragma once


#include <sys/types/string.h>


namespace checklib
{

struct accident_info
{
	std::uint32_t line;
	std::uint32_t col;
	sys::string file;
	sys::string errDesc;
};

struct accident_handler
{
	virtual void on_accident(const accident_info& info) = 0;
};

struct worker_intf
{
	virtual void check(const sys::string& file) = 0;
};

}
