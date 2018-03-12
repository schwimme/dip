#pragma once

#include "configurable_fsm_factory.h"
#include <base_intf/Algorithm/algorithm.h>


namespace checker
{


base::fsm::context_id configurable_fsm_ctx_factory::select_context(crossmodule::enumerator<base::fsm::context_id>* const allContexts) const
{
	// Extract vector from cross module safe type:
	std::vector<base::fsm::context_id> all_ctxs_without_invalid;
	const base::fsm::context_id* iter = nullptr;
	while (iter = allContexts->get())
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
		throw "No priorities given";
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
				if (base::find(all_ctxs_without_invalid, *ctxIter))
				{
					return *ctxIter;
				}
				++ctxIter;
			}

			// ASSERT(false); // ctxIter must be found at least one time in all_ctxs_without_invalid.
		}
	}

	throw "cannot found given contexts in one priority group";
}


bool configurable_fsm_ctx_factory::verify_priority_group(const std::vector<token_id>& priorityGroup, const std::vector<base::fsm::context_id>& ctxToVerify) const
{
	// oneGroup is group where all priorities must be - verify:
	for (const auto& ctx : ctxToVerify)
	{
		if (base::find(priorityGroup, ctx) == false)
		{
			return false;
		}
	}

	return true;
}


}
