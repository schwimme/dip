#pragma once


#include <base_intf/FiniteStateMachine/fsmwalker_intf.h>
#include <base_intf/PushDownAutomata/pda_walker_intf.h>
#include "checker_worker_intf.h"
#include <memory>
#include <vector>
#include "checker_intf/checker_intf.h"
#include "crossmodule/adapters/basestring.h"
#include "base_intf/ll_validator/ll_validator_intf.h"

namespace checklib
{


class worker:
	public worker_intf
{
public:
	struct factory
	{
		virtual std::shared_ptr<worker_intf> create_worker(std::shared_ptr<base::fsm_walker_intf> spFsmWalker, std::shared_ptr<base::ll_validator_walker_intf> spPdaWalker, incident_handler_intf& handler, const std::vector<uint32_t>& ignored_tokens) const
		{
			return std::make_shared<worker>(spFsmWalker, spPdaWalker, handler, ignored_tokens);
		}
	};

public:
	worker(std::shared_ptr<base::fsm_walker_intf> spFsmWalker, std::shared_ptr<base::ll_validator_walker_intf> spPdaWalker, incident_handler_intf& handler, const std::vector<uint32_t>& ignored_tokens):
		m_spFsmWalker(spFsmWalker),
		m_spPdaWalker(spPdaWalker),
		m_pHandler(&handler),
		m_ignored_tokens(ignored_tokens)
	{}

public:
	virtual void check(const sys::string& file) override;

protected:

// KTTODO - move else:
	using token_id = uint32_t;
	struct token
	{
		token_id id;
		sys::string value;
		uint32_t col;
		uint32_t line;
	};

	std::vector<token> parse(const sys::string& content);
	token create_token(const sys::string& value, token_id id);

	void check_syntax_analysis(const std::vector<token>& tokens);

	void report_incident(error_t err)
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

private:
	std::shared_ptr<base::fsm_walker_intf> m_spFsmWalker;
	std::shared_ptr<base::ll_validator_walker_intf> m_spPdaWalker;
	incident_handler_intf* m_pHandler = nullptr;
	sys::string m_file;
	std::vector<uint32_t> m_ignored_tokens;

	// position:
	std::uint32_t m_col = 1;
	std::uint32_t m_line = 1;
};

}
