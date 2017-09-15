#pragma once


#include "defs.h"


namespace Base
{
namespace Fsm 
{
	

struct IWalkable
{
	virtual const std::vector<Fsm::StateId>& GetNextStates(const Fsm::StateId& currentState, Fsm::AlphabetType ch) const = 0;
	virtual Fsm::ContextId GetContext(const Fsm::StateId& state) const = 0;
	virtual const Fsm::StateId& GetIdle() const = 0;

protected:
	IWalkable() {}
};


}
}