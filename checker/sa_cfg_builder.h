#pragma once


#include "sa_cfg_builder_intf.h"
#include <sys/parsing/parsing.h>
#include <checker_intf/checker_errors.h>
#include "base_intf/ll_validator/ll_validator_intf.h"


namespace checklib
{


class sa_cfg_builder:
	public sa_cfg_builder_intf,
	private sys::parsing_base<E_CHECKLIB_PARSING_ERROR>
{
public:
	struct factory
	{
		virtual std::shared_ptr<sa_cfg_builder_intf> create_sa_cfg_builder() const
		{
			return std::make_shared<sa_cfg_builder>();
		}
	};

public:
	virtual std::shared_ptr<sa_cfg> build(const sys::string& configuration) const override;

protected:
	sa_cfg::rule parse_one_rule(const sys::char_t*& p_cfg) const;
	la_cfg::token_id parse_token_id(const sys::char_t*& p_cfg) const;
	std::vector<base::rule_item> parse_stack_item_list(const sys::char_t*& p_cfg) const;
};


}
