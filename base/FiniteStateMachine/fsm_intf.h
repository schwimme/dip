#pragma once


#include "defs.h"
#include "fsmwalker_intf.h"
#include <memory>


namespace Base
{


struct IFsm
{
	//! \brief 'from'->epsilon->'to'.
	virtual void AddRule(const Fsm::StateId& from, const Fsm::StateId& to) = 0;

	//! \brief 'from'->'ch'->'to'.
	virtual void AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Fsm::AlphabetType ch) = 0;

	//! \brief 'from'->[a, b]->'to'.
	virtual void AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Fsm::AlphabetType a, Fsm::AlphabetType b) = 0;

	//! \brief Mark state as starting state.
	virtual void SetStart(const Fsm::StateId& state) = 0;

	//! \brief Generate new state id.
	virtual Fsm::StateId GenerateState(Fsm::ContextId ctx) = 0;

	/*!
		\brief		Create walker for current fsm.
		\warning	FSM cannot changed after optimization (AddRule, SetIdle and GenerateState are forbidden).
	*/
	virtual std::shared_ptr<IFsmWalker> CreateWalker() = 0;
};


}