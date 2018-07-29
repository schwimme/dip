#include "pda_intf_impl.h"
#include <crossmodule/adapters/vector.h>

namespace base
{
namespace pda
{


bool walker_intf_impl::process_step(const pda::token_id& input)
{
	return m_spWalker->process_step(input);
}


void walker_intf_impl::reset()
{
	m_spWalker->reset();
}


void walker_intf_impl::commit()
{
	m_spWalker->commit();
}


bool walker_intf_impl::is_accepted() const
{
	return m_spWalker->is_accepted();
}


void intf_impl::add_rule(const pda::token_id& input, const cross::enumerator<pda::stack_item>& stackTop, const cross::enumerator<pda::stack_item>& stackRewrite)
{
	m_impl.add_rule(input, enumerator_to_vector(stackTop), enumerator_to_vector(stackRewrite));
}


void intf_impl::create_walker(cross::versioned_intf_ref<pda_walker_intf> walker)
{
	pda_walker_intf* pIntf = new walker_intf_impl(m_impl.create_walker());
	if (walker.attach(pIntf) == false)
	{
		delete pIntf;
	}
}


}
}
