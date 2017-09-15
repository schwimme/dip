#pragma once


#include "fsmwalker_intf.h"
#include <vector>
#include "defs.h"
#include "walkable_intf.h"


namespace Base
{


class CFsmWalker:
	public IFsmWalker
{
public:
	CFsmWalker(Fsm::IWalkable& walkable);
	virtual bool ProcessStep(Fsm::AlphabetType ch) override;
	virtual bool VerifyLiteral(const String& literal) override;
	virtual void Reset() override;
	virtual bool IsInFiniteState() override;

private:
	std::vector<Fsm::StateId>	m_actualState;
	Fsm::IWalkable*				m_pWalkable;
};


}
