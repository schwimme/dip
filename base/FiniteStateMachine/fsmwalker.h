#pragma once


#include "fsmwalker_intf.h"
#include <vector>
#include <memory>
#include "defs.h"
#include "walkable_intf.h"
#include "fsmcontextfactory_intf.h"


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
	virtual bool ProcessStep(AlphabetType ch) override;

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
