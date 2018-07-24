#pragma once


#include <sys/types/string.h>
#include "cfa_specifier_intf.h"
#include <memory>


#include "token_specifier_intf.h"
#include "base_intf/ll_validator/ll_validator_intf.h"


namespace ccc
{

class cfa_specifier:
	public cfa_specifier_intf
{
public:
	struct factory
	{
		virtual std::shared_ptr<cfa_specifier_intf> create_cfa_specifier(const sys::string& configuration) const
		{
			auto sp_impl = std::make_shared<cfa_specifier>();
			sp_impl->initialize(configuration);
			return sp_impl;
		}
	};

private:
	enum class stack_item_e :
		uint32_t
	{
		// Consume symbols
		/* 0*/ si_space,
		/* 1*/ si_semicolon,
		/* 2*/ si_tab,
		/* 3*/ si_brace_close,
		/* 4*/ si_visibility,
		/* 5*/ si_id,
		/* 6*/ si_enter,
		/* 7*/ si_colon,

		// Class:
		/* 8*/ si_class,
		/* 9*/ si_class_impl,

		// Inherity:
		/* a*/ si_inheritance,
		/* b*/ si_inheritance_optional,

		// KTTODO sort:
		/* c*/ si_cascaded_id,
		/* d*/ si_cascaded_id_optional,

	};

	struct rule
	{
		base::rule_item m_parent;
		std::vector<base::rule_item> m_successors;
	};

	using rules = std::vector<rule>;

public:
	virtual sys::string serialize() const override;

private:
	void initialize(const sys::string& configuration);

private:
	rules m_rules;
};

}
