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

	build_consume_symbols();

	build_class();
	build_inheritance();
	build_cascaded_id();
}


void cfa_specifier::build_consume_symbols()
{
	m_rules.push_back(std::move(rule{ token_descriptor_e::w_space,       { stack_item_e::si_space},        {} }));
	m_rules.push_back(std::move(rule{ token_descriptor_e::o_semicolon,   { stack_item_e::si_semicolon },   {} }));
	m_rules.push_back(std::move(rule{ token_descriptor_e::w_tab,         { stack_item_e::si_tab },         {} }));
	m_rules.push_back(std::move(rule{ token_descriptor_e::p_brace_close, { stack_item_e::si_brace_close }, {} }));
	m_rules.push_back(std::move(rule{ token_descriptor_e::k_public,      { stack_item_e::si_visibility },  {} }));
	m_rules.push_back(std::move(rule{ token_descriptor_e::k_protected,   { stack_item_e::si_visibility },  {} }));
	m_rules.push_back(std::move(rule{ token_descriptor_e::k_private,     { stack_item_e::si_visibility },  {} }));
	m_rules.push_back(std::move(rule{ token_descriptor_e::i_basic,       { stack_item_e::si_id },          {} }));
	m_rules.push_back(std::move(rule{ token_descriptor_e::w_enter,       { stack_item_e::si_enter },       {} }));
	m_rules.push_back(std::move(rule{ token_descriptor_e::o_colon,       { stack_item_e::si_colon },       {} }));
}


void cfa_specifier::build_class()
{
	m_rules.push_back(std::move(rule
	{
		token_descriptor_e::k_class,
		{ stack_item_e::si_class },
		{ stack_item_e::si_space, stack_item_e::si_id, stack_item_e::si_inheritance, stack_item_e::si_class_impl }
	}));

	m_rules.push_back(std::move(rule
	{
		token_descriptor_e::k_class,
		{ },
		{ stack_item_e::si_space, stack_item_e::si_id, stack_item_e::si_inheritance, stack_item_e::si_class_impl }
	}));

	m_rules.push_back(std::move(rule
	{
		token_descriptor_e::p_brace_open,
		{ stack_item_e::si_class_impl },
		{ stack_item_e::si_brace_close, stack_item_e::si_semicolon, stack_item_e::si_enter }
	}));
}


void cfa_specifier::build_inheritance()
{
	m_rules.push_back(std::move(rule
	{
		token_descriptor_e::o_colon,
		{ stack_item_e::si_inheritance },
		{ stack_item_e::si_enter, stack_item_e::si_tab, stack_item_e::si_visibility, stack_item_e::si_space, stack_item_e::si_cascaded_id, stack_item_e::si_inheritance_optional }
	}));

	m_rules.push_back(std::move(rule
	{
		token_descriptor_e::o_comma,
		{ stack_item_e::si_inheritance_optional },
		{ stack_item_e::si_enter, stack_item_e::si_tab, stack_item_e::si_visibility, stack_item_e::si_space, stack_item_e::si_cascaded_id, stack_item_e::si_inheritance_optional }
	}));

	m_rules.push_back(std::move(rule
	{
		token_descriptor_e::w_enter,
		{ stack_item_e::si_inheritance_optional },
		{ }
	}));
}


void cfa_specifier::build_cascaded_id()
{
	m_rules.push_back(std::move(rule
	{
		token_descriptor_e::i_basic,
		{ stack_item_e::si_cascaded_id },
		{ stack_item_e::si_cascaded_id_optional }
	}));

	m_rules.push_back(std::move(rule
	{
		token_descriptor_e::o_colon,
		{ stack_item_e::si_cascaded_id_optional },
		{ stack_item_e::si_colon, stack_item_e::si_cascaded_id }
	}));

	m_rules.push_back(std::move(rule
	{
		token_descriptor_e::w_enter,
		{ stack_item_e::si_cascaded_id_optional },
		{ }
	}));

	m_rules.push_back(std::move(rule
	{
		token_descriptor_e::w_enter,
		{ stack_item_e::si_cascaded_id_optional, stack_item_e::si_inheritance_optional },
		{}
	}));
	
	m_rules.push_back(std::move(rule
	{
		token_descriptor_e::o_comma,
		{ stack_item_e::si_cascaded_id_optional, stack_item_e::si_inheritance_optional },
		{ stack_item_e::si_enter, stack_item_e::si_tab, stack_item_e::si_visibility, stack_item_e::si_space, stack_item_e::si_cascaded_id, stack_item_e::si_inheritance_optional }
	}));
}


}
