#pragma once


#include <base_intf/FiniteStateMachine/fsmwalker_intf.h>
#include <base_intf/PushDownAutomata/pda_walker_intf.h>
#include "checker_worker_intf.h"
#include <memory>
#include <vector>

namespace checker
{


class worker:
	public worker_intf
{
public:
	struct factory
	{
		virtual std::shared_ptr<worker_intf> create_worker(std::shared_ptr<base::fsm_walker_intf> spFsmWalker, std::shared_ptr<base::pda_walker_intf> spPdaWalker, accident_handler& handler) const
		{
			return std::make_shared<worker>(spFsmWalker, spPdaWalker, handler);
		}
	};

public:
	worker(std::shared_ptr<base::fsm_walker_intf> spFsmWalker, std::shared_ptr<base::pda_walker_intf> spPdaWalker, accident_handler& handler):
		m_spFsmWalker(spFsmWalker),
		m_spPdaWalker(spPdaWalker),
		m_pHandler(&handler)
	{}

public:
	virtual void check(const base::string& file) override;

protected:
	virtual // KTTODO - virtual because of tests - use base::file_intf
	base::string read_file_content() const;

// KTTODO - move else:
	using token_id = uint32_t;
	struct token
	{
		token_id id;
		base::string value;
		uint32_t col;
		uint32_t line;
	};

	std::vector<token> parse(const base::string& content);
	token create_token(const base::string& value, token_id id);

	void check_syntax_analysis(const std::vector<token>& tokens);

private:
	std::shared_ptr<base::fsm_walker_intf> m_spFsmWalker;
	std::shared_ptr<base::pda_walker_intf> m_spPdaWalker;
	accident_handler* m_pHandler = nullptr;
	base::string m_file;

	// position:
	std::uint32_t m_col = 1;
	std::uint32_t m_line = 1;
};

}
