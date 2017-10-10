#pragma once


#include <base_intf/FiniteStateMachine/fsmdefs.h>


namespace Base
{
namespace Fsm
{


//! \brief	Generate unique state id.
class CStateIdGenerator
{
public:
	static StateId Generate();

private:
	static uint32_t m_nextId;
};


}
}