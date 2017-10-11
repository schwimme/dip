#include "LexicalAnalysis.h"
#include <base_intf/Algorithm/algorithm.h>

namespace Checker
{

// KTTODO - make as normal class:

struct CConfigurableFsmCtxFactory :
	public Base::IFsmContextFactory
{
	static const Base::Fsm::ContextId INVALID_CTX = 0;

	virtual Base::Fsm::ContextId SelectContext(const std::vector<Base::Fsm::ContextId>& allContexts) const override
	{
		bool hasInvalidCtx = Base::Find(allContexts, INVALID_CTX);

		// Remove all invalid cotexts as they have lowest priority:
		std::vector<Base::Fsm::ContextId> ctxWithoutInvalid;
		for (const auto& i : allContexts)
		{
			if (i != INVALID_CTX)
			{
				ctxWithoutInvalid.push_back(i);
			}
		}

		// Just invalid ctxs given:
		if (ctxWithoutInvalid.size() == 0)
		{
			return INVALID_CTX;
		}

		// One ctx remaining:
		if (ctxWithoutInvalid.size() == 1)
		{
			return ctxWithoutInvalid[0];
		}

		// Identify priority group:
		for (const auto& oneGroup : m_priorities)
		{
			if (Base::Find(oneGroup, ctxWithoutInvalid[0]))
			{
				// oneGroup is group where all priorities must be - verify:
				for (const auto& ctx : ctxWithoutInvalid)
				{
					if (Base::Find(oneGroup, ctx) == false)
					{
						throw "KTTODO - ctx is not in priority group with others";
					}
				}

				// Ctx with higher priority are in front of m_priorities:
				LexicalAnalysisConfiguration::PriorityGroup::const_iterator ctxIter = oneGroup.begin();
				while (ctxIter != oneGroup.end())
				{
					if (Base::Find(ctxWithoutInvalid, *ctxIter))
					{
						return *ctxIter;
					}
					++ctxIter;
				}

				// ASSERT(false); // ctxIter must be found at least one time in ctxWithoutInvalid.
			}
		}

		throw "KTTODO - not specified priority group for given ctx";
	}

private:
	std::vector<LexicalAnalysisConfiguration::PriorityGroup>	m_priorities;
};


std::shared_ptr<Base::IFsmContextFactory> CLexicalAnalysis::CreateConfigurableFsmCtxFactory(const std::vector<LexicalAnalysisConfiguration::PriorityGroup>&) const
{
	return std::make_shared<CConfigurableFsmCtxFactory>();
}


void CLexicalAnalysis::BuildFsm(const LexicalAnalysisConfiguration& configuration)
{
	// Create factory:
	std::shared_ptr<Base::IFsmContextFactory> spCtxFactory = CreateConfigurableFsmCtxFactory(configuration.priority);

	// Create fsm using factory:
	m_spFsm = CreateFsm(spCtxFactory);
	Base::Fsm::StateId fsmStart = m_spFsm->GenerateState(CConfigurableFsmCtxFactory::INVALID_CTX);
	m_spFsm->SetStart(fsmStart);

	for (const auto& d : configuration.definition)
	{
		m_spFsm->AddRegex(fsmStart, d.second, d.first, CConfigurableFsmCtxFactory::INVALID_CTX);
	}
}


Token CLexicalAnalysis::CreateToken(const Base::String::const_iterator tokenBegin, const Base::String::const_iterator tokenEnd, Base::Fsm::ContextId id) const
{
	if (id == CConfigurableFsmCtxFactory::INVALID_CTX)
	{
		throw "KTTODO - parse error";
	}

	Base::String tokenValue(tokenBegin, tokenEnd);

	return Token(id, tokenValue);
}


std::vector<Token> CLexicalAnalysis::Parse(const Base::String& input)
{
	// Prepare output:
	std::vector<Token> tokens;

	// Create walker:
	std::shared_ptr<Base::IFsmWalker> spWalker = m_spFsm->CreateWalker();

	// Help iterators:
	Base::String::const_iterator actualPosition = input.begin();
	Base::String::const_iterator actualTokenBegin = input.begin();

	while (actualPosition != input.end())
	{
		bool successStep = spWalker->ProcessStep(*actualPosition);

		if (successStep == true)
		{
			++actualPosition;
		}
		else
		{
			// End of known token:
			Token t = CreateToken(actualTokenBegin, actualPosition, spWalker->GetContext());

			spWalker->Reset();

			tokens.push_back(std::move(t));

			// Mark new token begin:
			actualTokenBegin = actualPosition;
		}
	}

	Token t = CreateToken(actualTokenBegin, actualPosition, spWalker->GetContext());
	tokens.push_back(std::move(t));

	return tokens;
}


}
