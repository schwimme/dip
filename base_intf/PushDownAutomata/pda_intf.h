#pragma once


#include "pda_defs.h"
#include "pda_walker_intf.h"
#include <crossmodule/types/enumerator.h>
#include <crossmodule/types/query.h>
#include <crossmodule/types/pointer.h>


namespace base
{


struct pda_intf:
	cross::queryable<0xf8d06fc9>
{
	static constexpr uint32_t version = 1;

	virtual void add_rule(const pda::token_id& input, const cross::enumerator<pda::stack_item>& stackTop, const cross::enumerator<pda::stack_item>& stackRewrite) = 0;
	virtual void create_walker(cross::versioned_intf_ref<pda_walker_intf> walker) = 0;
};


}
