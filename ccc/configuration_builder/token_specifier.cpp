#include "token_specifier.h"
#include "sys\debugging\debug.h"
#include "error\exceptions.h"


namespace ccc
{
namespace
{
	const sys::char_t ignore_all[] = { '*', '(', '-', '(', 1, -1, ')', ')' };
}

void token_specifier::initialize(const sys::string& configuration)
{
	if (configuration == TEXT("cpp"))
	{
		build_cpp();
	}
	else if (configuration == TEXT("ps"))
	{
		build_ps();
	}
	else
	{
		ASSERT(false);
		throw exception_t(-9);
	}
}


void token_specifier::build_cpp()
{
	m_tokens =
	{
		{ token_descriptor_e::k_do,                TEXT("do") },
		{ token_descriptor_e::k_while,             TEXT("while") },
		{ token_descriptor_e::k_if,                TEXT("if") },
		{ token_descriptor_e::k_else,              TEXT("else") },
		{ token_descriptor_e::k_for,	           TEXT("for") },
		{ token_descriptor_e::k_class,	           TEXT("?(.(class).(struct))") },
		{ token_descriptor_e::k_enum,	           TEXT("enum") },
		{ token_descriptor_e::k_virtual,           TEXT("virtual") },
		{ token_descriptor_e::k_const,	           TEXT("const") },
		{ token_descriptor_e::k_override,          TEXT("override") },
		{ token_descriptor_e::k_namespace,         TEXT("namespace") },
		{ token_descriptor_e::k_public,            TEXT("public") },
		{ token_descriptor_e::k_protected,         TEXT("protected") },
		{ token_descriptor_e::k_private,           TEXT("private") },
		{ token_descriptor_e::o_less,              TEXT("<") },
		{ token_descriptor_e::o_greater,	       TEXT(">") },
		{ token_descriptor_e::o_plus,		       TEXT("\\+") },
		{ token_descriptor_e::o_minus,		       TEXT("\\-") },
		{ token_descriptor_e::o_star,		       TEXT("\\*") },
		{ token_descriptor_e::o_slash,		       TEXT("/") },
		{ token_descriptor_e::o_back_slash,	       TEXT("\\\\") },
		{ token_descriptor_e::o_comma,		       TEXT(",") },
		{ token_descriptor_e::o_dot,		       TEXT("\\.") },
		{ token_descriptor_e::o_colon,		       TEXT(":") },
		{ token_descriptor_e::o_semicolon,	       TEXT(";") },
		{ token_descriptor_e::o_equal,		       TEXT("=") },
		{ token_descriptor_e::o_question,	       TEXT("\\?") },
		{ token_descriptor_e::o_exclamation,       TEXT("\\!") },
		{ token_descriptor_e::o_or,                TEXT("\\|") },
		{ token_descriptor_e::o_and,               TEXT("\\&") },
		{ token_descriptor_e::n_numeric,           TEXT("?(0.(!(\\-)-(19)*(-(09))!(\\.*(-(09)))))") },
		{ token_descriptor_e::i_basic,	           TEXT("+(?(-(az)_))") },
		{ token_descriptor_e::i_member,	           TEXT("m_+(?(-(az)_))") },
		{ token_descriptor_e::w_enter,	           TEXT("?(.(\n).(\r\n))") },
		{ token_descriptor_e::w_space,	           TEXT(" ") },
		{ token_descriptor_e::w_tab,	           TEXT("\t") },
		{ token_descriptor_e::p_guard,             TEXT("#pragma once") },
		{ token_descriptor_e::p_include_absolute,  TEXT("#include <*(+(-(az))/)+(-(az))\\.h>") },
		{ token_descriptor_e::p_include_relative,  TEXT("#include \\\"*(+(-(az))/)+(-(az))\\.h\\\"") },
		{ token_descriptor_e::v_string,            TEXT("\\\"*(?(-(az)-(AZ)_%\\- -(09)\\?\\!\\:'))\\\"") },
		{ token_descriptor_e::p_brace_open,        TEXT("{") },
		{ token_descriptor_e::p_brace_close,       TEXT("}") },
		{ token_descriptor_e::p_parenthesis_open,  TEXT("\\(") },
		{ token_descriptor_e::p_parenthesis_close, TEXT("\\)") },
		{ token_descriptor_e::p_square_open,       TEXT("[") },
		{ token_descriptor_e::p_square_close,      TEXT("]") },
		// { token_descriptor_e::ignored_1, sys::string(TEXT("//")) + ignore_all + TEXT("?(.(\n).(\r\n))") },
		// { token_descriptor_e::ignored_2, sys::string(TEXT("/\\*")) + ignore_all + TEXT("\\*/") },
		// { token_descriptor_e::ignored_3, sys::string(TEXT("?(.(\n).(\r\n))// ccc_ignore_begin")) + ignore_all + TEXT("?(.(\n).(\r\n))// ccc_ignore_end") },

	};

	m_priorities =
	{
		{ token_descriptor_e::k_do,        token_descriptor_e::i_basic },
		{ token_descriptor_e::k_while,	   token_descriptor_e::i_basic },
		{ token_descriptor_e::k_if,		   token_descriptor_e::i_basic },
		{ token_descriptor_e::k_else,	   token_descriptor_e::i_basic },
		{ token_descriptor_e::k_for,	   token_descriptor_e::i_basic },
		{ token_descriptor_e::k_class,	   token_descriptor_e::i_basic },
		{ token_descriptor_e::k_enum,	   token_descriptor_e::i_basic },
		{ token_descriptor_e::k_virtual,   token_descriptor_e::i_basic },
		{ token_descriptor_e::k_const,	   token_descriptor_e::i_basic },
		{ token_descriptor_e::k_override,  token_descriptor_e::i_basic },
		{ token_descriptor_e::k_public,    token_descriptor_e::i_basic },
		{ token_descriptor_e::k_protected, token_descriptor_e::i_basic },
		{ token_descriptor_e::k_private,   token_descriptor_e::i_basic },
		{ token_descriptor_e::i_member,    token_descriptor_e::i_basic },
		{ token_descriptor_e::o_minus,     token_descriptor_e::n_numeric }
	};
}


void token_specifier::build_ps()
{
	m_tokens = 
	{
		{ token_descriptor_e::n_numeric,           TEXT("?(0.(!(\\-)-(19)*(-(09))!(\\.*(-(09)))))") },
		{ token_descriptor_e::i_basic,	           TEXT("$+(?(-(az)_))") },
		{ token_descriptor_e::i_function_call_ps,  TEXT("-(AZ)+(?(-(az)-))") },
		{ token_descriptor_e::w_enter,	           TEXT("?(.(\n).(\r\n))") },
		{ token_descriptor_e::w_space,	           TEXT(" ") },
		{ token_descriptor_e::w_tab,	           TEXT("\t") },
		{ token_descriptor_e::p_brace_open,        TEXT("{") },
		{ token_descriptor_e::p_brace_close,       TEXT("}") },
		{ token_descriptor_e::p_parenthesis_open,  TEXT("\\(") },
		{ token_descriptor_e::p_parenthesis_close, TEXT("\\)") },
		{ token_descriptor_e::p_square_open,       TEXT("[") },
		{ token_descriptor_e::p_square_close,      TEXT("]") },
		{ token_descriptor_e::i_function_param_ps, TEXT("\\-?(-(az)-(AZ)") },
		{ token_descriptor_e::v_string,            TEXT("\\\"*(?(-(az)-(AZ)_%\\- -(09)\\?\\!\\:'))\\\"") },
		{ token_descriptor_e::o_equal,		       TEXT("=") }
	};
}


const sys::string token_specifier::serialize(std::vector<uint32_t>& ignored_tokens) const
{
	ignored_tokens = { (uint32_t)token_descriptor_e::ignored_1 };

	// {[{1,"r1"},{2,"r2"}],[[1,2],[3,4,5,6]]}

	auto remove_last_char = 
	[](sys::string& str, sys::char_t c)
	{
		if (str[str.size() - 1] == c)
		{
			str.pop_back();
		}
	};

	sys::string rv = TEXT("{[");
	for (const auto& it : m_tokens)
	{
		rv += TEXT('{') + sys::to_string((uint32_t)it.first) + TEXT(",\"") + it.second + TEXT("\"},");
	}

	remove_last_char(rv, TEXT(','));

	rv += TEXT("],[");
	for (const auto& it : m_priorities)
	{
		rv += TEXT('[');

		for (const auto& pit : it)
		{
			rv += sys::to_string((uint32_t)pit) + TEXT(",");
		}
		remove_last_char(rv, TEXT(','));

		rv += TEXT("],");
	}

	remove_last_char(rv, TEXT(','));
	
	rv += TEXT("]}");

	return rv;
}


}
