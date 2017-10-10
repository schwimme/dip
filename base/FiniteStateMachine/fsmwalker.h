#pragma once


#include <base_intf/FiniteStateMachine/fsmwalker_intf.h>
#include <base_intf/FiniteStateMachine/fsmcontextfactory_intf.h>
#include <base_intf/FiniteStateMachine/fsmdefs.h>

#include "walkable_intf.h"

#include <vector>
#include <memory>


namespace Base
{
namespace Fsm
{

//! \brief Class for walking fsm.
class CWalker:
	public IFsmWalker
{
public:
	CWalker(IWalkable& walkable, std::shared_ptr<IFsmContextFactory>& spCtxFactory);

	//! \copydoc IFsmWalker::ProcessStep
	virtual bool ProcessStep(Base::CharType ch) override;

	//! \copydoc IFsmWalker::VerifyLiteral
	virtual bool VerifyLiteral(const String& literal) override;

	//! \copydoc IFsmWalker::Reset
	virtual void Reset() override;

	//! \copydoc IFsmWalker::GetContext
	virtual Fsm::ContextId GetContext() const override;

private:
	std::vector<Fsm::StateId>			m_actualState;
	std::shared_ptr<IFsmContextFactory> m_spCtxFactory;
	IWalkable*							m_pWalkable;
};


}
}
