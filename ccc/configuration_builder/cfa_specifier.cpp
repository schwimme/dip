#include "cfa_specifier.h"


namespace ccc
{

sys::string cfa_specifier::serialize() const
{
	auto remove_last_char =
	[](sys::string& str, sys::char_t c)
	{
		if (str[str.size() - 1] == c)
		{
			str.pop_back();
		}
	};

	sys::string rv = TEXT("[");

	// [{0,[26,9],[]},{5,[],[1243]}]
	for (const auto& one_rule : m_rules)
	{
		rv += TEXT('{') + sys::to_string((uint32_t)one_rule.m_token) + TEXT(",[");

		for (const auto& top : one_rule.m_stack_top)
		{
			rv += sys::to_string((uint32_t)top) + TEXT(",");
		}

		remove_last_char(rv, TEXT(','));
		rv += TEXT("],[");

		for (const auto& rep : one_rule.m_stack_replace)
		{
			rv += sys::to_string((uint32_t)rep) + TEXT(",");
		}

		remove_last_char(rv, TEXT(','));
		rv += TEXT("]},");
	}

	remove_last_char(rv, TEXT(','));
	
	rv += TEXT(']');

	return rv;
}


void cfa_specifier::initialize(const sys::string& configuration)
{
	(void)(configuration);
	build_class();
	build_inheritance();
}


void cfa_specifier::build_class()
{
	// Expected class:
	rule without_inheritance
	{
		token_descriptor_e::k_class,
		{ stack_item_e::si_class },
		{ stack_item_e::si_space, stack_item_e::si_id, stack_item_e::si_enter, stack_item_e::si_class_impl }
	};

	rule with_inheritance
	{
		token_descriptor_e::k_class,
		{ stack_item_e::si_class },
		{ stack_item_e::si_space, stack_item_e::si_id, stack_item_e::si_inheritance, stack_item_e::si_class_impl }
	};

	// Unexpected class:
	rule without_inheritance_unex
	{
		token_descriptor_e::k_class,
		{},
		{ stack_item_e::si_space, stack_item_e::si_id, stack_item_e::si_enter, stack_item_e::si_class_impl }
	};

	rule with_inheritance_unex
	{
		token_descriptor_e::k_class,
		{ },
		{ stack_item_e::si_space, stack_item_e::si_id, stack_item_e::si_inheritance, stack_item_e::si_class_impl }
	};


	m_rules.push_back(std::move(without_inheritance));
	m_rules.push_back(std::move(with_inheritance));

	m_rules.push_back(std::move(without_inheritance_unex));
	m_rules.push_back(std::move(with_inheritance_unex));
}


void cfa_specifier::build_inheritance()
{
	// Expected inheritance:
	rule inheritance
	{
		token_descriptor_e::o_colon,
		{ stack_item_e::si_inheritance },
		{ stack_item_e::si_inheritance_impl}
	};

	rule inheritance_impl
	{
		token_descriptor_e::w_enter,
		{ stack_item_e::si_inheritance_impl },
		{ stack_item_e::si_enter, stack_item_e::si_tab, stack_item_e::si_visibility, stack_item_e::si_cascaded_id, stack_item_e::si_inheritance_optional }
	};

	rule inheritance_optional_continue
	{
		token_descriptor_e::o_comma,
		{ stack_item_e::si_inheritance_optional },
		{ stack_item_e::si_inheritance_impl }
	};

	rule inheritance_optional_break
	{
		token_descriptor_e::w_enter,
		{ stack_item_e::si_inheritance_optional },
		{ }
	};
}

}
