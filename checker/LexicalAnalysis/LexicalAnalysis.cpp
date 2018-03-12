#include "LexicalAnalysis.h"
#include <base_intf/Algorithm/algorithm.h>
#include <crossmodule/adapters/basestring.h>


namespace checker
{

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
