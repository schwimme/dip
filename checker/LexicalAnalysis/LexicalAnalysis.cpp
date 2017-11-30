#include "LexicalAnalysis.h"
#include <base_intf/Algorithm/algorithm.h>
#include <crossmodule/adapters/basestring.h>


namespace checker
{

// KTTODO - make as normal class:

struct configurable_fsm_ctx_factory:
	public base::fsm_context_factory_intf
{
	configurable_fsm_ctx_factory(const std::vector<lexical_analysis_configuration::priority_group>& priorities):
		m_priorities(priorities)
	{}

	static const base::fsm::context_id INVALID_CTX = -1;

	virtual base::fsm::context_id select_context(crossmodule::enumerator<base::fsm::context_id>* const allContexts) const override
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

		// Identify priority group:
		for (const auto& oneGroup : m_priorities)
		{
			if (verify_priority_group(oneGroup, all_ctxs_without_invalid))
			{
				// Ctx with higher priority are in front of m_priorities:
				lexical_analysis_configuration::priority_group::const_iterator ctxIter = oneGroup.begin();
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

protected:
	bool verify_priority_group(const lexical_analysis_configuration::priority_group& priorityGroup, const std::vector<base::fsm::context_id>& ctxToVerify) const
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

private:
	std::vector<lexical_analysis_configuration::priority_group>	m_priorities;
};


std::shared_ptr<base::fsm_context_factory_intf> lexical_analysis::create_configurable_fsm_ctx_factory(const std::vector<lexical_analysis_configuration::priority_group>& priorities) const
{
	return std::make_shared<configurable_fsm_ctx_factory>(priorities);
}


void lexical_analysis::configure(const lexical_analysis_configuration& configuration)
{
	// Get base:
	void* pBase;
	if (m_objectFactory.get_object(GUID_BASE_V1, &pBase) != 0) // KTTODO - no error
	{
		throw "fail to get base";
	}
	m_spBase = std::shared_ptr<base::base_intf>((base::base_intf*)pBase);

	// Create factory:
	std::shared_ptr<base::fsm_context_factory_intf> spCtxFactory = create_configurable_fsm_ctx_factory(configuration.priority);

	// Create fsm using factory:
	if (m_spBase->create_fsm(m_spFsm, spCtxFactory) != 0) // KTTODO - no error
	{
		throw "fail to get fsm from base";
	}

	base::fsm::state_id fsmStart = m_spFsm->generate_state(configurable_fsm_ctx_factory::INVALID_CTX);
	m_spFsm->set_start(fsmStart);

	for (const auto& d : configuration.definition)
	{
		m_spFsm->add_regex(fsmStart, crossmodule::base_string_on_string_ref(d.second), d.first, configurable_fsm_ctx_factory::INVALID_CTX);
	}
}


token lexical_analysis::create_token(const base::string::const_iterator tokenBegin, const base::string::const_iterator tokenEnd, base::fsm::context_id id) const
{
	if (id == configurable_fsm_ctx_factory::INVALID_CTX)
	{
		throw "KTTODO - parse error";
	}

	base::string tokenValue(tokenBegin, tokenEnd);

	return token(id, tokenValue);
}


std::vector<token> lexical_analysis::parse(const base::string& input)
{
	// Prepare output:
	std::vector<token> tokens;

	// Create walker:
	std::shared_ptr<base::fsm_walker_intf> spWalker = m_spFsm->create_walker();

	// Help iterators:
	base::string::const_iterator actualPosition = input.begin();
	base::string::const_iterator actualTokenBegin = input.begin();

	while (actualPosition != input.end())
	{
		bool successStep = spWalker->process_step(*actualPosition);

		if (successStep == true)
		{
			++actualPosition;
		}
		else
		{
			// End of known token:
			token t = create_token(actualTokenBegin, actualPosition, spWalker->get_context());

			spWalker->reset();

			tokens.push_back(std::move(t));

			// Mark new token begin:
			actualTokenBegin = actualPosition;
		}
	}

	token t = create_token(actualTokenBegin, actualPosition, spWalker->get_context());
	tokens.push_back(std::move(t));

	return tokens;
}


}
