#pragma once


#include "pda_defs.h"
#include <crossmodule/types/query.h>


namespace base
{


struct pda_walker_intf:
	cross::queryable<0x4c95508c>
{
	static constexpr uint32_t version = 1;

	virtual bool process_step(const pda::token_id& input) = 0;
	virtual void reset() = 0;
	virtual void commit() = 0;
	virtual bool is_accepted() const = 0;
};


}