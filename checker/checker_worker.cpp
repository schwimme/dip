#include "checker_worker.h"
#include "crossmodule\adapters\basestring.h"

#include <iostream>
#include <fstream>
#include <string>


namespace checklib
{

sys::string worker::read_file_content() const
{
	std::string res;
	std::string line;
	std::ifstream myfile(m_file.c_str());
	if (myfile.is_open == false) return "";

	while (std::getline(myfile, line))
	{
		res += line;
	}

	return res;
}
void worker::check(const sys::string& file)
{
	m_file = file;
	sys::string content = read_file_content();

	std::vector<token> t = parse(content);

	check_syntax_analysis(t);
}


worker::token worker::create_token(const sys::string& value, token_id id)
{
	if (id == 0) // KTTODO token id
	{
		report_incident(incident_handler_intf::incident_type::unrecognized_token);
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
			// End of known token:
			token t{
				m_spFsmWalker->get_context(),
				sys::string(actualTokenBegin, actualPosition),
				m_col,
				m_line
			};

			m_spFsmWalker->reset();

			tokens.push_back(std::move(t));

			// Mark new token begin:
			actualTokenBegin = actualPosition;
		}
	}

	token t{
		m_spFsmWalker->get_context(),
		sys::string(actualTokenBegin, actualPosition),
		m_col,
		m_line
	};
	tokens.push_back(std::move(t));

	return tokens;
}


void worker::check_syntax_analysis(const std::vector<worker::token>& tokens)
{
	size_t tokens_size = tokens.size();

	std::vector<worker::token>::const_iterator it = tokens.begin();
	for (size_t i = 0; i < tokens_size - 1; ++i)
	{
		bool is_cfg_empty = !m_spPdaWalker->process_step(it->id);

		if (is_cfg_empty)
		{
			report_incident(incident_handler_intf::incident_type::no_rule);
		}
		else
		{
			m_spPdaWalker->commit();
		}

		it++;
	}

	m_spPdaWalker->process_step(it->id);
	m_spPdaWalker->commit();

	if (m_spPdaWalker->is_accepted() == false)
	{
		report_incident(incident_handler_intf::incident_type::unexpected_end_of_file);
	}

	m_pHandler->on_finish(cross::string_ref(m_file.data(), m_file.size()), E_NO_ERROR);
	return;
}

}
