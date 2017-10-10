#include "LexicalAnalysis.h"


// KTTODO - make as normal class:
std::shared_ptr<Base::IFsmContextFactory> CLexicalAnalysis::CreateConfigurableFsmCtxFactory(const std::vector<LexicalAnalysisConfiguration::PriorityGroup>&) const
{
	class CBaseContextFactory:
		public Base::IFsmContextFactory
	{
	public:
		virtual Fsm::ContextId SelectContext(const std::vector<Fsm::ContextId>& allContexts) const override
		{
			if (Base::Find(allContexts, 1))
			{
				return 1;
			}

			return 0;
		}
	};
}


void CLexicalAnalysis::BuildFsm(const LexicalAnalysisConfiguration& configuration)
{
}

std::vector<Token> CLexicalAnalysis::Parse(const Base::String& input)
{

}

