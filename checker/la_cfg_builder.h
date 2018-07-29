#pragma once


#include "la_cfg_builder_intf.h"
#include <sys/parsing/parsing.h>
#include <checker_intf/checker_errors.h>


namespace checklib
{


class la_cfg_builder:
	public la_cfg_builder_intf,
	private sys::parsing_base<E_CHECKLIB_PARSING_ERROR>
{
public:
	struct factory
	{
		virtual std::shared_ptr<la_cfg_builder_intf> create_la_cfg_builder() const
		{
			return std::make_shared<la_cfg_builder>();
		}
	};

public:
	virtual std::shared_ptr<la_cfg> build(const sys::string& configuration) const override;

protected:
	std::pair<token_id, sys::string> parse_token_pair(const sys::char_t*& p_cfg) const;

	token_id parse_token_id(const sys::char_t*& p_cfg) const;

	sys::string parse_token_regex(const sys::char_t*& p_cfg) const;

	std::vector<token_id> parse_priority_group(const sys::char_t*& p_cfg) const;
};


}
