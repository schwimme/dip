#include "cfa_specifier.h"


namespace ccc
{

namespace
{

const base::rule_item_non_terminal
	n_program(0x1),
	n_headers(0x2),
	n_class_def(0x3),
	n_headers_one(0x4),
	n_headers_rest(0x5),
	n_inheritance(0x6),
	n_class_def_impl(0x7),
	n_inheritance_one(0x8),
	n_inheritance_rest(0x9),
	n_structured_id(0xa),
	n_structured_id_rest(0xb),
	n_class_impl(0xc),
	n_class_impl_line(0xd),
	n_class_impl_rest(0xe),
	n_params_decl(0xf),
	n_params_decl_one(0x10),
	n_params_decl_rest(0x11),
	n_class_impl_id_one(0x12),
	n_class_impl_id_rest(0x13),
	n_visibility(0x14),
	n_function_rest(0x15),
	n_function_one(0x16),
	n_function_impl(0x17),
	n_function_impl_lines(0x18),
	n_impl_line(0x19),
	n_function_def(0x1a),
	n_function_impl_end(0x1b),
	n_function_impl_wrapper(0x1c),
	n_statement(0x1d),
	n_impl_line_end(0x1e),
	n_statement_impl(0x1f),
	n_statement_impl_control(0x20),
	n_expression_lv(0x21),
	n_expression_definition(0x22),
	n_expression_definition_rest(0x23),
	n_params_call(0x24),
	n_params_call_one(0x25),
	n_params_call_rest(0x26),
	n_virtuality_specifier(0x27),
	n_expression_rv(0x28),
	n_expression_rv_rest(0x29),
	_n_end(1000);

base::rule_item_terminal from_descriptor(ccc::token_descriptor_e d)
{
	return (uint32_t)d;
}

#define TOKEN(t) from_descriptor(token_descriptor_e:: t )

}


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

	// [{0,[26,9]},{5,[1324]}]
	sys::string rv = TEXT("[");

	for (const auto& one_rule : m_rules)
	{
		rv += TEXT('{') + sys::to_string(one_rule.m_parent.value) + TEXT(",[");

		for (const auto& top : one_rule.m_successors)
		{
			rv += top.is_terminal ? TEXT('t') : TEXT('n');
			rv += sys::to_string(top.value) + TEXT(",");
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

	// .h files
	m_rules.push_back({ n_program, { TOKEN(p_guard), TOKEN(w_enter), TOKEN(w_enter), n_headers, TOKEN(w_enter), n_class_def } });

	m_rules.push_back({ n_headers,{ n_headers_one, n_headers_rest } });
	m_rules.push_back({ n_headers_one,{ TOKEN(p_include_absolute), TOKEN(w_enter) } });
	m_rules.push_back({ n_headers_one,{ TOKEN(p_include_relative), TOKEN(w_enter) } });

	m_rules.push_back({ n_headers_rest,{ n_headers_one, n_headers_rest } });
	m_rules.push_back({ n_headers_rest,{ TOKEN(w_enter) } });

	m_rules.push_back({ n_class_def, { TOKEN(k_class), TOKEN(w_space), TOKEN(i_basic), base::meta::indent_inc(), n_inheritance, base::meta::indent_dec(), n_class_def_impl } });

	m_rules.push_back({ n_inheritance,{ TOKEN(w_enter)} });
	m_rules.push_back({ n_inheritance,{ TOKEN(o_colon), n_inheritance_one, n_inheritance_rest } });
	m_rules.push_back({ n_inheritance_one,{ TOKEN(w_enter), base::meta::indent_check(), n_visibility, TOKEN(w_space), TOKEN(i_basic) } });
	m_rules.push_back({ n_inheritance_rest,{ TOKEN(o_comma), n_inheritance_one, n_inheritance_rest } });
	m_rules.push_back({ n_inheritance_rest,{ TOKEN(w_enter) } });

	m_rules.push_back({ n_structured_id,{ TOKEN(i_basic), n_structured_id_rest } });
	m_rules.push_back({ n_structured_id_rest,{ TOKEN(o_colon), TOKEN(o_colon), n_structured_id } });
	m_rules.push_back({ n_structured_id_rest,{ TOKEN(o_less), TOKEN(i_basic), TOKEN(o_greater), n_structured_id_rest } });
	m_rules.push_back({ n_structured_id_rest,{ TOKEN(w_enter) } });
	m_rules.push_back({ n_structured_id_rest,{ TOKEN(o_comma) } });
	m_rules.push_back({ n_structured_id_rest,{ TOKEN(w_space) } });

	m_rules.push_back({ n_class_def_impl,{ TOKEN(p_brace_open), n_class_impl, TOKEN(o_semicolon), TOKEN(w_enter) } });
	m_rules.push_back({ n_class_impl,{ TOKEN(p_brace_close) } });
	m_rules.push_back({ n_class_impl,{ TOKEN(w_enter), n_class_impl_line, n_class_impl_rest } });
	m_rules.push_back({ n_class_impl_line,{ TOKEN(w_tab), n_function_def} });
	m_rules.push_back({ n_class_impl_rest,{ TOKEN(p_brace_close) } });
	m_rules.push_back({ n_class_impl_rest,{ n_class_impl_line, n_class_impl_rest } });
	m_rules.push_back({ n_class_impl_rest,{ TOKEN(w_enter), n_visibility, TOKEN(o_colon), TOKEN(w_enter), n_class_impl_id_rest } });

	m_rules.push_back({ n_function_def,{ n_structured_id, TOKEN(i_basic), TOKEN(p_parenthesis_open), n_params_decl, TOKEN(o_semicolon), TOKEN(w_enter) } });
	m_rules.push_back({ n_function_def,{ TOKEN(k_virtual), TOKEN(w_space), n_structured_id, TOKEN(i_basic), TOKEN(p_parenthesis_open), n_params_decl, TOKEN(w_space), n_virtuality_specifier, TOKEN(o_semicolon), TOKEN(w_enter) } });

	m_rules.push_back({ n_function_def,{ TOKEN(k_override) } });
	m_rules.push_back({ n_function_def,{ TOKEN(o_equal), TOKEN(w_space), TOKEN(n_numeric) } });

	m_rules.push_back({ n_class_impl_id_one,{ TOKEN(w_tab), n_structured_id, TOKEN(i_member), TOKEN(o_semicolon), TOKEN(w_enter)} });
	m_rules.push_back({ n_class_impl_id_rest,{ TOKEN(p_brace_close) } });
	m_rules.push_back({ n_class_impl_id_rest,{ n_class_impl_id_one, n_class_impl_id_rest } });

	m_rules.push_back({ n_params_decl,{ TOKEN(p_parenthesis_close) } });
	m_rules.push_back({ n_params_decl,{ n_params_decl_one, n_params_decl_rest } });
	m_rules.push_back({ n_params_decl_one,{ TOKEN(i_basic), TOKEN(w_space), TOKEN(i_basic) } });
	m_rules.push_back({ n_params_decl_rest,{ TOKEN(o_comma), TOKEN(w_space), n_params_decl_one, n_params_decl_rest } });
	m_rules.push_back({ n_params_decl_rest,{ TOKEN(p_parenthesis_close) } });

	m_rules.push_back({ n_visibility,{ TOKEN(k_public) } });
	m_rules.push_back({ n_visibility,{ TOKEN(k_protected) } });
	m_rules.push_back({ n_visibility,{ TOKEN(k_private) } });

	// .cpp files
	m_rules.push_back({ n_program,{ n_headers, TOKEN(w_enter), n_function_rest} });

	m_rules.push_back({ n_function_rest,{ TOKEN(w_enter) } });
	m_rules.push_back({ n_function_rest,{ n_function_one, TOKEN(w_enter), TOKEN(w_enter), n_function_rest } });
	m_rules.push_back({ n_function_one,{ n_structured_id, TOKEN(i_basic), TOKEN(o_colon), TOKEN(o_colon), TOKEN(i_basic), TOKEN(p_parenthesis_open), n_params_decl, TOKEN(w_enter), n_function_impl_wrapper } });

	m_rules.push_back({ n_function_impl_wrapper,{ TOKEN(p_brace_open), TOKEN(w_enter), n_impl_line } });

	m_rules.push_back({ n_impl_line,{ TOKEN(w_tab), n_statement } });
	m_rules.push_back({ n_impl_line,{ TOKEN(p_brace_close), n_impl_line_end } });

	m_rules.push_back({ n_impl_line_end,{ TOKEN(o_semicolon) } });
	m_rules.push_back({ n_impl_line_end,{ TOKEN(w_enter), base::meta::indent_dec(), base::meta::indent_check(), n_impl_line } });

	m_rules.push_back({ n_statement,{ TOKEN(p_brace_open), TOKEN(w_enter), base::meta::indent_inc(), base::meta::indent_check(), n_impl_line } });
	m_rules.push_back({ n_statement,{ n_statement_impl, base::meta::indent_check(), n_impl_line } });

	// Statements:

	m_rules.push_back({ n_params_call,{ TOKEN(p_parenthesis_close) } });
	m_rules.push_back({ n_params_call,{ n_params_call_one, n_params_call_rest } });
	m_rules.push_back({ n_params_call_one,{ TOKEN(i_basic) } });
	m_rules.push_back({ n_params_call_rest,{ TOKEN(o_comma), TOKEN(w_space), n_params_call_one, n_params_call_rest } });
	m_rules.push_back({ n_params_call_rest,{ TOKEN(p_parenthesis_close) } });

	m_rules.push_back({ n_statement_impl,{ n_expression_lv, TOKEN(w_enter) } });
	m_rules.push_back({ n_statement_impl,{ TOKEN(w_enter) } });

	m_rules.push_back({ n_expression_lv, { n_structured_id, n_expression_definition } });
	m_rules.push_back({ n_expression_definition,{ TOKEN(i_basic), n_expression_definition_rest } });
	m_rules.push_back({ n_expression_definition,{ TOKEN(o_equal), TOKEN(w_space), n_expression_rv } });

	m_rules.push_back({ n_expression_definition_rest,{ TOKEN(o_semicolon) } });
	m_rules.push_back({ n_expression_definition_rest,{ TOKEN(p_parenthesis_open), n_params_call, TOKEN(o_semicolon) } });
	m_rules.push_back({ n_expression_definition_rest,{ TOKEN(w_space), TOKEN(o_equal), TOKEN(w_space), n_expression_rv } });

	m_rules.push_back({ n_expression_rv,{ TOKEN(i_basic), TOKEN(o_semicolon) } });
	m_rules.push_back({ n_expression_rv,{ TOKEN(p_parenthesis_open), TOKEN(i_basic) } });
}

}

#undef TOKEN
