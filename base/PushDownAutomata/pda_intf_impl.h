#pragma once


#include <base_intf/PushDownAutomata/pda_intf.h>
#include <base_intf/PushDownAutomata/pda_walker_intf.h>
#include "pda_impl.h"
#include "pda_walker_impl.h"
#include <vector>
#include <memory>

namespace base
{
namespace pda
{


class walker_intf_impl:
	public crossmodule::query_impl<pda_walker_intf>
{
public:
	walker_intf_impl(std::shared_ptr<pda::walker_impl> spWalker):
		m_spWalker(spWalker)
	{}

public:
	virtual bool process_step(const pda::token_id& input) override;
	virtual void reset() override;
	virtual bool accepted() const override;

private:
	std::shared_ptr<pda::walker_impl> m_spWalker;
};


class intf_impl:
	public pda_intf
{
public:
	virtual void register_rule(const pda::token_id& input, const crossmodule::enumerator<pda::stack_item>& stackTop, const crossmodule::enumerator<pda::stack_item>& stackRewrite) override;
	virtual void create_walker(crossmodule::versioned_intf_ref<pda_walker_intf> walker) override;

private:
	impl m_impl;
};


}


}
