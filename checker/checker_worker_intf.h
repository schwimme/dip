#pragma once


#include <sys/types/string.h>


namespace checklib
{

struct worker_intf
{
	virtual void check(const sys::string& file) = 0;
};

}
