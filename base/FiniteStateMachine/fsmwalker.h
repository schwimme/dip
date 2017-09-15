#pragma once


#include "fsmwalker_intf.h"
#include <vector>
#include <memory>
#include "defs.h"
#include "walkable_intf.h"
#include "fsmcontextfactory_intf.h"


namespace Base
{


class CFsmWalker:
	public IFsmWalker
{
public:
	CFsmWalker(Fsm::IWalkable& walkable, std::shared_ptr<IFsmContextFactory>& spCtxFactory);
	virtual bool ProcessStep(Fsm::AlphabetType ch) override;
	virtual bool VerifyLiteral(const String& literal) override;
	virtual void Reset() override;
	virtual Fsm::ContextId GetContext() const override;

private:
	std::vector<Fsm::StateId>			m_actualState;
	std::shared_ptr<IFsmContextFactory> m_spCtxFactory;
	Fsm::IWalkable*						m_pWalkable;
};


}
