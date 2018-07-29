#pragma once


#include <base_intf/FiniteStateMachine/fsmwalker_intf.h>
#include <base_intf/FiniteStateMachine/fsmcontextfactory_intf.h>
#include <base_intf/FiniteStateMachine/fsmdefs.h>

#include "walkable_intf.h"

#include <vector>
#include <memory>


namespace base
{
namespace fsm
{

//! \brief Class for walking fsm.
class walker:
	public fsm_walker_intf
{
public:
	walker(walkable_intf& walkable, std::shared_ptr<fsm_context_factory_intf>& spCtxFactory);

	//! \copydoc IFsmWalker::ProcessStep
	virtual bool process_step(sys::char_t ch) override;

	//! \copydoc IFsmWalker::VerifyLiteral
	virtual bool verify_literal(cross::string_ref literal) override;

	//! \copydoc IFsmWalker::Reset
	virtual void reset() override;

	//! \copydoc IFsmWalker::GetContext
	virtual fsm::context_id get_context() const override;

private:
	std::vector<fsm::state_id>					m_actualState;
	std::shared_ptr<fsm_context_factory_intf>	m_spCtxFactory;
	walkable_intf*								m_pWalkable;
};


}
}
