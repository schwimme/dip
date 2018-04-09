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
	public cross::query_impl<pda_walker_intf>
{
public:
	walker_intf_impl(std::shared_ptr<pda::walker_impl> spWalker):
		m_spWalker(spWalker)
	{}

public:
	virtual bool process_step(const pda::token_id& input) override;
	virtual void reset() override;
	virtual void commit() override;
	virtual bool is_accepted() const override;

private:
	std::shared_ptr<pda::walker_impl> m_spWalker;
};


class intf_impl:
	public cross::query_impl<pda_intf>
{
public:
	virtual void add_rule(const pda::token_id& input, const cross::enumerator<pda::stack_item>& stackTop, const cross::enumerator<pda::stack_item>& stackRewrite) override;
	virtual void create_walker(cross::versioned_intf_ref<pda_walker_intf> walker) override;

private:
	impl m_impl;
};


}


}
