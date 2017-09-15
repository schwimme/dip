#include "stdafx.h"
#include "CppUnitTest.h"

#include <base/FiniteStateMachine/fsm.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Base_Test
{		

TEST_CLASS(Fsm_test)
{
public:

TEST_METHOD(test1)
{
	auto fsm = Base::CFsm::Factory().CreateFiniteStateMachine();

	Base::Fsm::StateId s0 = fsm->GenerateState(Base::Fsm::NON_FINITE);
	Base::Fsm::StateId s1 = fsm->GenerateState(Base::Fsm::NON_FINITE);
	Base::Fsm::StateId s2 = fsm->GenerateState(Base::Fsm::NON_FINITE);
	Base::Fsm::StateId s3 = fsm->GenerateState(Base::Fsm::FINITE);

	fsm->SetStart(s0);

	fsm->AddRule(s0, s0);
	fsm->AddRule(s0, s1);
	fsm->AddRule(s1, s2, TEXT('b'));

	auto w = fsm->CreateWalker();
	w->VerifyLiteral(TEXT("aa"));
	Base::Fsm::ContextId d = w->GetContext();
}


};
}