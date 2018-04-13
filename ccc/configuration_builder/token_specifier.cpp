#include "token_specifier.h"


namespace ccc
{


void token_specifier::initialize(const sys::string& input_configuration)
{
	// Currently unused
	(void)(input_configuration);

	build_keywords();
	build_operators();
	build_numbers();
	build_identificators();
	build_whitespaces();

	build_priorities();
}


void token_specifier::build_keywords()
{
	static const std::vector<std::pair<token_descriptor_e, sys::string>> key_words = 
	{
		{ token_descriptor_e::k_do,		   TEXT("do")},
		{ token_descriptor_e::k_while,	   TEXT("while")},
		{ token_descriptor_e::k_if,		   TEXT("if")},
		{ token_descriptor_e::k_else,	   TEXT("else")},
		{ token_descriptor_e::k_for,	   TEXT("for")},
		{ token_descriptor_e::k_class,	   TEXT("?(.(class).(struct))")},
		{ token_descriptor_e::k_enum,	   TEXT("enum")},
		{ token_descriptor_e::k_virtual,   TEXT("virtual")},
		{ token_descriptor_e::k_const,	   TEXT("const")},
		{ token_descriptor_e::k_override,  TEXT("override")},
		{ token_descriptor_e::k_namespace, TEXT("namespace")},
		{ token_descriptor_e::k_public,    TEXT("public")},
		{ token_descriptor_e::k_protected, TEXT("protected") },
		{ token_descriptor_e::k_private,   TEXT("private") },
	};

	m_tokens.insert(m_tokens.end(), key_words.begin(), key_words.end());
}


void token_specifier::build_operators()
{
	static const std::vector<std::pair<token_descriptor_e, sys::string>> operators =
	{
		{ token_descriptor_e::o_less,		 TEXT("<") },
		{ token_descriptor_e::o_greater,	 TEXT(">") },
		{ token_descriptor_e::o_plus,		 TEXT("\\+") },
		{ token_descriptor_e::o_minus,		 TEXT("\\-") },
		{ token_descriptor_e::o_star,		 TEXT("\\*") },
		{ token_descriptor_e::o_slash,		 TEXT("/") },
		{ token_descriptor_e::o_back_slash,	 TEXT("\\\\") },
		{ token_descriptor_e::o_comma,		 TEXT(",") },
		{ token_descriptor_e::o_dot,		 TEXT("\\.") },
		{ token_descriptor_e::o_colon,		 TEXT(":") },
		{ token_descriptor_e::o_semicolon,	 TEXT(";") },
		{ token_descriptor_e::o_equal,		 TEXT("=") },
		{ token_descriptor_e::o_question,	 TEXT("\\?") },
		{ token_descriptor_e::o_exclamation, TEXT("\\!") }
	};

	m_tokens.insert(m_tokens.end(), operators.begin(), operators.end());
}


void token_specifier::build_numbers()
{
	static const std::pair<token_descriptor_e, sys::string> numbers =
	{
		token_descriptor_e::n_numeric, TEXT("(-(19)*(09)!(.(\\.-(19)*(09)))")
	};

	m_tokens.push_back(numbers);
}


void token_specifier::build_identificators()
{
	static const std::vector<std::pair<token_descriptor_e, sys::string>> identificators =
	{
		{ token_descriptor_e::i_basic,	 TEXT("+(-(az))") },
		{ token_descriptor_e::i_member,	 TEXT("m_+(-(az))") },
	};

	m_tokens.insert(m_tokens.end(), identificators.begin(), identificators.end());
}


void token_specifier::build_priorities()
{
	static const std::vector<std::vector<token_descriptor_e>> priorities =
	{
		{ token_descriptor_e::k_do,		  token_descriptor_e::i_basic },
		{ token_descriptor_e::k_while,	  token_descriptor_e::i_basic },
		{ token_descriptor_e::k_if,		  token_descriptor_e::i_basic },
		{ token_descriptor_e::k_else,	  token_descriptor_e::i_basic },
		{ token_descriptor_e::k_for,	  token_descriptor_e::i_basic },
		{ token_descriptor_e::k_class,	  token_descriptor_e::i_basic },
		{ token_descriptor_e::k_enum,	  token_descriptor_e::i_basic },
		{ token_descriptor_e::k_virtual,  token_descriptor_e::i_basic },
		{ token_descriptor_e::k_const,	  token_descriptor_e::i_basic },
		{ token_descriptor_e::k_override, token_descriptor_e::i_basic },
		{ token_descriptor_e::i_member,   token_descriptor_e::i_basic },
	};

	m_priorities = priorities;
}


void token_specifier::build_whitespaces()
{
	static const std::vector<std::pair<token_descriptor_e, sys::string>> identificators =
	{
		{ token_descriptor_e::w_enter,	 TEXT("?(.(\n).(\r\n))") },
		{ token_descriptor_e::w_space,	 TEXT(" ") },
		{ token_descriptor_e::w_tab,	 TEXT("\t") },
	};

	m_tokens.insert(m_tokens.end(), identificators.begin(), identificators.end());
}

const sys::string token_specifier::serialize() const
{
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
