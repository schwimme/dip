#pragma once

#include "defs.h"

namespace Base
{
namespace Fsm
{


class CStateIdGenerator
{
public:
	static StateId Generate();

private:
	static uint32_t m_nextId;
};


}
}