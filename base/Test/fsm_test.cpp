#include "stdafx.h"
#include "CppUnitTest.h"

#include <base/FiniteStateMachine/fsm.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Base_Test
{		

TEST_CLASS(Fsm_test)
{
public:

	struct BuilderTest :
		public Base::Fsm::CBuilder
	{
		virtual Base::Fsm::CBuilder::Regex ParseRegex(const Base::String& regex) const override
		{
			return m_fakeRegex;
		}

		using RType = Base::Fsm::CBuilder::RegexItem::Type;
		using RItem = Base::Fsm::CBuilder::RegexItem;
		Base::Fsm::CBuilder::Regex m_fakeRegex;
	};

TEST_METHOD(KTTODO_All_UT)
{
	BuilderTest b;
	BuilderTest::RItem r;
	r.char1 = 'a';
	b.m_fakeRegex.push_back(r);
	r.iteration_begin = 0;
	r.type = BuilderTest::RType::ITERATION;
	b.m_fakeRegex.push_back(r);
	r.char1 = 'c';
	r.type = BuilderTest::RType::NORMAL;
	b.m_fakeRegex.push_back(r);

	std::shared_ptr<Base::IFsmContextFactory> f = std::make_shared<Base::Fsm::CBaseContextFactory>();
	Base::CFsm fsm(f);
	auto a = fsm.GenerateState(1);
	fsm.SetStart(a);
	b.MergeRegex(fsm, TEXT(""), 1, 0);

	auto w = fsm.CreateWalker();
	Assert::IsTrue(w->VerifyLiteral(TEXT("c")));
}


};
}