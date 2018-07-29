#pragma once

#include "configurable_fsm_factory.h"
#include <sys/algorithm/algorithm.h>
#include "error/exceptions.h"
#include "sys/debugging/debug.h"


namespace checklib
{


base::fsm::context_id configurable_fsm_ctx_factory::select_context(cross::enumerator<base::fsm::context_id>* const allContexts) const
{
	// Extract vector from cross module safe type:
	std::vector<base::fsm::context_id> all_ctxs_without_invalid;
	const base::fsm::context_id* iter = nullptr;
	while ((iter = allContexts->get()) != nullptr)
	{
		if (*iter != INVALID_CTX)
		{
			all_ctxs_without_invalid.push_back(*iter);
		}
		allContexts->next();
	}

	// Just invalid ctxs given:
	if (all_ctxs_without_invalid.size() == 0)
	{
		return INVALID_CTX;
	}

	// One ctx remaining:
	if (all_ctxs_without_invalid.size() == 1)
	{
		return all_ctxs_without_invalid[0];
	}

	if (m_priorities.size() == 0)
	{
		ASSERT(false);
		throw exception_t(-1);
	}

	// Identify priority group:
	for (const auto& oneGroup : m_priorities)
	{
		if (verify_priority_group(oneGroup, all_ctxs_without_invalid))
		{
			// Ctx with higher priority are in front of m_priorities:
			auto ctxIter = oneGroup.begin();
			while (ctxIter != oneGroup.end())
			{
				if (sys::find(all_ctxs_without_invalid, *ctxIter))
				{
					return *ctxIter;
				}
				++ctxIter;
			}
		}
	}

	ASSERT(false);
	throw exception_t(-2);
}


bool configurable_fsm_ctx_factory::verify_priority_group(const std::vector<token_id>& priorityGroup, const std::vector<base::fsm::context_id>& ctxToVerify) const
{
	// oneGroup is group where all priorities must be - verify:
	for (const auto& ctx : ctxToVerify)
	{
		if (sys::find(priorityGroup, ctx) == false)
		{
			return false;
		}
	}

	return true;
}


}
