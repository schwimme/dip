#pragma once


#include "checker_errors.h"
#include <crossmodule/types/query.h>
#include <crossmodule/types/string.h>
#include <crossmodule/types/enumerator.h>


namespace checklib
{

struct checker_intf:
	cross::queryable<0x14a7d539>
{
	static constexpr uint32_t version = 1;

	/*!	\brief Configure checker by given configuration.
		\param[in]	path	Specifies if given params are paths to text files or content itself.
	*/
	virtual error_t configure(cross::string_ref la_config, cross::string_ref sa_config, bool path) = 0;

	/*!	\brief Check given files.
		KTTODO - callback with errors
	*/
	virtual error_t check(cross::enumerator<cross::string_ref>* pFiles) = 0;
};

}
