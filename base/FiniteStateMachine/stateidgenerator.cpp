#include "stateidgenerator.h"


namespace base
{
namespace fsm
{


state_id state_id_generator::m_nextId = 0;


state_id state_id_generator::generate()
{
	return m_nextId++;
}


}
}