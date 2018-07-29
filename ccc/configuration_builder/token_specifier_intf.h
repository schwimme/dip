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
	/* 0*/ k_do,
	/* 1*/ k_while,
	/* 2*/ k_if,
	/* 3*/ k_else,
	/* 4*/ k_for,
	/* 5*/ k_class,
	/* 6*/ k_enum,
	/* 7*/ k_namespace,
	/* 8*/ k_virtual,
	/* 9*/ k_const,
	/* a*/ k_override,
	/* b*/ k_public,
	/* c*/ k_protected,
	/* d*/ k_private,

	     // Operators
	/* e*/ o_less,
	/* f*/ o_greater,
	/*10*/ o_plus,
	/*11*/ o_minus,
	/*12*/ o_star,
	/*13*/ o_slash,
	/*14*/ o_back_slash,
	/*15*/ o_comma,
	/*16*/ o_dot,
	/*17*/ o_colon,
	/*18*/ o_semicolon,
	/*19*/ o_equal,
	/*1a*/ o_question,
	/*1b*/ o_exclamation,

	       // Numbers
	/*1c*/ n_numeric,

	       // Identificators
	/*1d*/ i_basic,
	/*1e*/ i_member,

	       // Whitespaces:
	/*1f*/ w_enter,
	/*20*/ w_tab,
	/*21*/ w_space,

	       // Parenthesis:
	/*22*/ p_brace_open,
	/*23*/ p_brace_close,
	/*24*/ p_parenthesis_open,
	/*25*/ p_parenthesis_close,
	/*26*/ p_square_open,
	/*27*/ p_square_close,

	/*28*/ ignored_1,
	/*29*/ ignored_2,
	/*2a*/ ignored_3,

	// directives:
	/*2b*/ p_guard,
	/*2c*/ p_include_absolute,
	/*2d*/ p_include_relative,

	/*2e*/ o_or,
	/*2f*/ o_and,

	/*30*/ v_string,
	/*31*/ i_function_call_ps,
	/*32*/ i_function_param_ps,
	/*33*/ k_function,
	/*34*/ k_param,
	/*35*/ k_switch,
	/*36*/ k_string,
	/*37*/ k_int,
};

struct token_specifier_intf
{
	virtual const sys::string serialize(std::vector<uint32_t>& ignored_tokens) const = 0;
};

}