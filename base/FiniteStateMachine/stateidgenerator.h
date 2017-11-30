#pragma once


#include <base_intf/FiniteStateMachine/fsmdefs.h>


namespace base
{
namespace fsm
{


//! \brief	Generate unique state id.
class state_id_generator
{
public:
	static state_id generate();

private:
	static state_id m_nextId;
};


}
}