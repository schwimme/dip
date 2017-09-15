#include "stateidgenerator.h"


namespace Base
{
namespace Fsm
{


uint32_t CStateIdGenerator::m_nextId = 0;


StateId CStateIdGenerator::Generate()
{
	return Base::ToString(m_nextId++);
}


}
}