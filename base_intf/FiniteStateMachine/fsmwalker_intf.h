#pragma once


#include "fsmdefs.h"
#include <crossmodule/types/string.h>


namespace base
{
	
	
struct fsm_walker_intf
{
	//! \brief Process step in automat by given character.
	virtual bool process_step(base::char_t ch) = 0;

	/*!
		\brief	Verify entire literal.
		\return	true if literal was readed successfuly,
		\return	false otherwise.
	*/
	virtual bool verify_literal(crossmodule::string_ref literal) = 0;

	//! \brief	Reset to initial state.
	virtual void reset() = 0;

	//! \brief	Get context of actual state.
	virtual fsm::context_id get_context() const = 0;
};
	

}
