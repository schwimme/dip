#pragma once


#include "LexicalAnalysis_intf.h"
#include <memory>


#include <base_intf/BaseFactory.h>


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
	virtual void BuildFsm(const LexicalAnalysisConfiguration& configuration) = 0;

	//! \copydoc ILexicalAnalysis::Parse
	virtual std::vector<Token> Parse(const Base::String& input) = 0;

protected:
	// KTTODO - make as normal class:
	virtual std::shared_ptr<Base::IFsmContextFactory> CreateConfigurableFsmCtxFactory(const std::vector<LexicalAnalysisConfiguration::PriorityGroup>& priorities) const;

private:
	std::shared_ptr<Base::IFsm>	m_spFsm;
};
