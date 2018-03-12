#pragma once


#include <base_intf/FiniteStateMachine/fsmcontextfactory_intf.h>
#include <vector>

namespace checker
{

// KTTODO move else:
using token_id = uint32_t;


class configurable_fsm_ctx_factory :
	public base::fsm_context_factory_intf
{
public:
	static const base::fsm::context_id INVALID_CTX = -1;

public:
	configurable_fsm_ctx_factory(const std::vector<std::vector<token_id>>& priorities):
		m_priorities(priorities)
	{}

	virtual base::fsm::context_id select_context(crossmodule::enumerator<base::fsm::context_id>* const allContexts) const override;

protected:
	bool verify_priority_group(const std::vector<token_id>& priorityGroup, const std::vector<base::fsm::context_id>& ctxToVerify) const;

private:
	std::vector<std::vector<token_id>>	m_priorities;
};

}
