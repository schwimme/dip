#pragma once


#include "LexicalAnalysis_intf.h"
#include <memory>
#include <unordered_map>

#include <base_intf/BaseFactory.h>


namespace Checker
{


class CLexicalAnalysis:
	public ILexicalAnalysis,
	protected Base::BaseFactory
{
public:
	struct Factory
	{
		virtual std::shared_ptr<ILexicalAnalysis> CreateLexicalAnalysis() const
		{
			return std::make_shared<CLexicalAnalysis>();
		}
	};

public: // ILexicalAnalysis

	//! \copydoc ILexicalAnalysis::BuildFsm
	virtual void BuildFsm(const LexicalAnalysisConfiguration& configuration) override;

	//! \copydoc ILexicalAnalysis::Parse
	virtual std::vector<Token> Parse(const Base::String& input) override;

protected:
	// KTTODO - make as normal class:
	virtual std::shared_ptr<Base::IFsmContextFactory> CreateConfigurableFsmCtxFactory(const std::vector<LexicalAnalysisConfiguration::PriorityGroup>& priorities) const;

	Token CreateToken(const Base::String::const_iterator tokenBegin, const Base::String::const_iterator tokenEnd, Base::Fsm::ContextId id) const;

private:
	std::shared_ptr<Base::IFsm> m_spFsm;
};


}