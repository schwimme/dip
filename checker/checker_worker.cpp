#include "checker_worker.h"

namespace checker
{

base::string worker::read_file_content() const
{
	//KTTODO  - impl : currently just faked:
	throw "Not implemented";
	return base::string();
}


void worker::check(const base::string& file)
{
	m_file = file;
	base::string content = read_file_content();

	std::vector<token> t = parse(content);
}


worker::token worker::create_token(const base::string& value, token_id id)
{
	if (id == 0) // KTTODO token id
	{
		accident_info i{m_line, m_col, m_file, base::string(TEXT("Invalid token - '")) + value + TEXT("'") };
		m_pHandler->on_accident(i);
	}

	return token{ id, value, m_col, m_line };
}


std::vector<worker::token> worker::parse(const base::string& content)
{
	std::vector<worker::token> tokens;

	// Help iterators:
	base::string::const_iterator actualPosition = content.begin();
	base::string::const_iterator actualTokenBegin = content.begin();

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
				base::string(actualTokenBegin, actualPosition), 
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
		base::string(actualTokenBegin, actualPosition),
		m_col,
		m_line
	};
	tokens.push_back(std::move(t));

	return tokens;
}


void worker::check_syntax_analysis(std::vector<worker::token>& tokens)
{
	for (const auto& i : tokens)
	{
		bool success = m_spPdaWalker->process_step(i.id);
		if (success)
		{
			m_spPdaWalker->commit();
		}
		else
		{
			accident_info ai{ i.line, i.col, m_file, base::string(TEXT("Unexpected token '")) + i.value + TEXT("' - skipped") };
			m_pHandler->on_accident(ai);
		}
	}
}

}
