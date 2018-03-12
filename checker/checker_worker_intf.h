#pragma once


#include <types/string.h>


namespace checker
{

struct accident_info
{
	std::uint32_t line;
	std::uint32_t col;
	base::string file;
	base::string errDesc;
};

struct accident_handler
{
	virtual void on_accident(const accident_info& info) = 0;
};

struct worker_intf
{
	virtual void check(const base::string& file) = 0;
};

}
