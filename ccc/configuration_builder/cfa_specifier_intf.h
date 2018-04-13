#pragma once


#include <sys/types/string.h>


namespace ccc
{

struct cfa_specifier_intf
{
	virtual sys::string serialize() const = 0;
};

}
