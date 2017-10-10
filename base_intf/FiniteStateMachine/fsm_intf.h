#pragma once


#include "fsmdefs.h"
#include "fsmwalker_intf.h"
#include <memory>


namespace Base
{


struct IFsm
{
	//! \brief 'from'->epsilon->'to'.
	virtual void AddRule(const Fsm::StateId& from, const Fsm::StateId& to) = 0;

	//! \brief 'from'->'ch'->'to'.
	virtual void AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Base::CharType ch) = 0;

	//! \brief 'from'->[a, b]->'to'.
	virtual void AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Base::CharType a, Base::CharType b) = 0;

	//! \brief 'from'->epsilon->regex.
	virtual void AddRegex(const Fsm::StateId& from, const Base::String& regex, Fsm::ContextId valid, Fsm::ContextId invalid) = 0;

	//! \brief Mark state as starting state.
	virtual void SetStart(const Fsm::StateId& state) = 0;

	//! \brief Generate new state id.
	virtual Fsm::StateId GenerateState(Fsm::ContextId ctx) = 0;

	/*!
		\brief		Create walker for current fsm.
		\warning	FSM cannot changed after optimization (AddRule, SetIdle and GenerateState are forbidden).
		\note		Walker does not change fsm structure so more instances can be used at the time. 
	*/
	virtual std::shared_ptr<IFsmWalker> CreateWalker() = 0;
};


}