#pragma once


#include "defs.h"


namespace Base
{
	
	
struct IFsmWalker
{
	//! \brief Process step in automat by given character.
	virtual bool ProcessStep(Fsm::AlphabetType ch) = 0;

	/*!
		\brief	Verify entire literal.
		\return	true if literal was readed successfuly,
		\return	false otherwise.
	*/
	virtual bool VerifyLiteral(const String& literal) = 0;

	//! \brief	Reset to initial state.
	virtual void Reset() = 0;

	//! \brief	Get context of actual state.
	virtual Fsm::ContextId GetContext() const = 0;
};
	

}
