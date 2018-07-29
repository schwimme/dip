#include "checker_worker.h"
#include <iostream>
#include <fstream>
#include <string>
#include "sys\scopeguard\scopeguard.h"
#include "sys\algorithm\file_reader.h"
#include "sys\algorithm\algorithm.h"

namespace checklib
{

void worker::check(const sys::string& file)
{
	m_file = file;
	auto finish_guard = sys::scope_guard([this]
	{
		m_pHandler->on_finish(cross::string_ref(m_file.data(), m_file.size()), E_NO_ERROR);
	});

	auto common_error_guard = sys::scope_guard([this]
	{
		report_incident(-8);
	});

	sys::string content = sys::read_file(m_file);
	std::vector<token> t = parse(content);
	check_syntax_analysis(t);

	common_error_guard.dismiss();
}


worker::token worker::create_token(const sys::string& value, token_id id)
{
	if (id == 0) // KTTODO token id
	{
		report_incident(-7);
	}

	return token{ id, value, m_col, m_line };
}


std::vector<worker::token> worker::parse(const sys::string& content)
{
	std::vector<worker::token> tokens;

	// Help iterators:
	sys::string::const_iterator actualPosition = content.begin();
	sys::string::const_iterator actualTokenBegin = content.begin();

	while (actualPosition != content.end())
	{
		bool successStep = m_spFsmWalker->process_step(*actualPosition);

		if (successStep == true)
		{
			if (*actualPosition == TEXT('\n'))
			{
				m_col = 1;
				m_line++;
			}
			else
			{
				m_col++;
			}

			++actualPosition;
		}
		else
		{
			if (actualPosition == actualTokenBegin)
			{
				report_incident(-7);
				++actualPosition;
				continue;
			}

			// End of known token:
			auto ctx = m_spFsmWalker->get_context();

			if (sys::find(m_ignored_tokens, ctx) == false)
			{
				token t{
					ctx,
					sys::string(actualTokenBegin, actualPosition),
					m_col,
					m_line
				};
				tokens.push_back(std::move(t));
			}

			m_spFsmWalker->reset();

			// Mark new token begin:
			actualTokenBegin = actualPosition;
		}
	}

	auto ctx = m_spFsmWalker->get_context();
	if (sys::find(m_ignored_tokens, ctx) == false)
	{
		token t{
			ctx,
			sys::string(actualTokenBegin, actualPosition),
			m_col,
			m_line
		};
		tokens.push_back(std::move(t));
	}
	return tokens;
}


void worker::check_syntax_analysis(const std::vector<worker::token>& tokens)
{
	for (const auto& it: tokens)
	{
		error_t rv = m_spPdaWalker->process_step(it.id);
		if (FAILED(rv))
		{
			report_incident(rv);
		}
	}
	
	if (m_spPdaWalker->is_accepted() == false)
	{
		report_incident(-6);
	}

	return;
}

void worker::report_incident(error_t err)
{
	incident_handler_intf::incident_type type = incident_handler_intf::incident_type::common_error;
	switch (err)
	{
	case -1:
		type = incident_handler_intf::incident_type::multiple_rules_registration;
		break;

	case -2:
		type = incident_handler_intf::incident_type::empty_stack;
		break;

	case -3:
		type = incident_handler_intf::incident_type::unexpected_token;
		break;

	case -4:
		type = incident_handler_intf::incident_type::no_rule;
		break;

	case -5:
		type = incident_handler_intf::incident_type::bad_indent;
		break;

	case -6:
		type = incident_handler_intf::incident_type::unexpected_end_of_file;
		break;

	case -7:
		type = incident_handler_intf::incident_type::unrecognized_token;
		break;

	default:
		break;
	}
	incident_handler_intf::incident_info info{ cross::sys_string_on_string_ref(m_file), m_col, m_line, type };

	incident_handler_intf::action action_to_take;
	sys::string replacement;
	cross::sys_string_settable_string_ref adapter(replacement);

	m_pHandler->on_incident(info, &action_to_take, &adapter);
}

}
