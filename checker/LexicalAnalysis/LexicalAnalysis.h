#pragma once


#include "LexicalAnalysis_intf.h"
#include <memory>
#include <unordered_map>
#include <base_intf/FiniteStateMachine/fsmcontextfactory_intf.h>
#include <base_intf/FiniteStateMachine/fsm_intf.h>
#include <crossmodule/factory/objectfactory.h>
#include <base/base_intf.h>

namespace checker
{


class lexical_analysis:
	public lexical_analysis_intf
{
public:
	struct factory
	{
		virtual std::shared_ptr<lexical_analysis_intf> create_lexical_analysis() const
		{
			return std::make_shared<lexical_analysis>();
		}
	};

public: // ILexicalAnalysis

	//! \copydoc ILexicalAnalysis::BuildFsm
	virtual void configure(const lexical_analysis_configuration& configuration) override;

	//! \copydoc ILexicalAnalysis::Parse
	virtual std::vector<token> parse(const base::string& input) override;

protected:
	// KTTODO - make as normal class:
	virtual std::shared_ptr<base::fsm_context_factory_intf> create_configurable_fsm_ctx_factory(const std::vector<lexical_analysis_configuration::priority_group>& priorities) const;

	token create_token(const base::string::const_iterator tokenBegin, const base::string::const_iterator tokenEnd, base::fsm::context_id id) const;

private:
	crossmodule::object_factory			m_objectFactory;
	std::shared_ptr<base::base_intf>	m_spBase;
	std::shared_ptr<base::fsm_intf>		m_spFsm;
};


}
