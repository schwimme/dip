#pragma once


#include <crossmodule/types/query.h>
#include <crossmodule/types/string.h>
#include <crossmodule/types/enumerator.h>

#include <sys/event_distributor/registrator_intf.h>

#include "ccc_errors.h"
#include "ccc_accident_handler.h"


namespace ccc
{

struct checking_module_intf:
	cross::queryable<0x0ff023ec>
{
	static constexpr uint32_t version = 1;

	virtual error_t configure(cross::string_ref config_path, sys::registrator_intf** ppRegistrator, accident_handler_intf* pHandler) = 0;
	virtual error_t check_files(cross::enumerator<cross::string_ref>* files) = 0;
};

}
