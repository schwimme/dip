#pragma once

#include <cstdint>
#include <utility>
#include <vector>


#include <sys/types/string.h>


namespace ccc
{

enum class token_descriptor_e:
	uint32_t
{
	// Keywords:
	k_do,
	k_while,
	k_if,
	k_else,
	k_for,
	k_class,
	k_enum,
	k_namespace,
	k_virtual,
	k_const,
	k_override,
	k_public,
	k_protected,
	k_private,

	// Operators
	o_less,
	o_greater,
	o_plus,
	o_minus,
	o_star,
	o_slash,
	o_back_slash,
	o_comma,
	o_dot,
	o_colon,
	o_semicolon,
	o_equal,
	o_question,
	o_exclamation,

	// Numbers
	n_numeric,

	// Identificators
	i_basic,
	i_member,

	// Whitespaces:
	w_enter,
	w_tab,
	w_space
};

struct token_specifier_intf
{
	virtual const sys::string serialize() const = 0;
};

}