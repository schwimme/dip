#pragma once


#include "checker_errors.h"
#include <crossmodule/types/query.h>
#include <crossmodule/types/string.h>
#include <crossmodule/types/enumerator.h>


namespace checklib
{

struct incident_handler_intf
{
	enum class action
	{
		ignore,
		replace,
		automatic
	};

	enum class incident_type
	{
		unrecognized_token,
		no_rule,
		unexpected_end_of_file
	};

	struct incident_info
	{
		cross::string_ref file;
		uint32_t col;
		uint32_t line;
		incident_type type;
	};

	virtual void on_incident(incident_info info, action* action_to_take, cross::settable_string_ref* replacement) noexcept = 0;
	virtual void on_finish(cross::string_ref file, error_t err) = 0;
};


struct checker_intf:
	cross::queryable<0x14a7d539>
{
	static constexpr uint32_t version = 1;

	/*!	\brief Configure checker by given configuration.
	*/
	virtual error_t configure(incident_handler_intf* pHandler, cross::string_ref la_config, cross::string_ref sa_config) = 0;

	/*!	\brief Check given files.
		KTTODO - callback with errors
	*/
	virtual error_t check(cross::enumerator<cross::string_ref>* pFiles) = 0;
};

}
