#include "stdafx.h"
#include "CppUnitTest.h"

#include <base/FiniteStateMachine/fsm.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Base_Test
{		

TEST_CLASS(Fsm_test)
{
public:

TEST_METHOD(Email_validation)
{
	auto fsm = Base::CFsm::Factory().CreateFiniteStateMachine();

	Base::Fsm::StateId name1 = fsm->GenerateState(Base::Fsm::NON_FINITE);
	Base::Fsm::StateId name2 = fsm->GenerateState(Base::Fsm::NON_FINITE);
	Base::Fsm::StateId domain1 = fsm->GenerateState(Base::Fsm::NON_FINITE);
	Base::Fsm::StateId domain2 = fsm->GenerateState(Base::Fsm::NON_FINITE);
	Base::Fsm::StateId code1 = fsm->GenerateState(Base::Fsm::NON_FINITE);
	Base::Fsm::StateId code2 = fsm->GenerateState(Base::Fsm::FINITE);

	fsm->SetStart(name1);

	fsm->AddRule(name1, name2, TEXT('a'), TEXT('z'));
	fsm->AddRule(name2, name2, TEXT('a'), TEXT('z'));
	fsm->AddRule(name2, domain1, TEXT('@'));
	fsm->AddRule(domain1, domain2, TEXT('a'), TEXT('z'));
	fsm->AddRule(domain2, domain2, TEXT('a'), TEXT('z'));
	fsm->AddRule(domain2, code1, TEXT('.'));
	fsm->AddRule(code1, code2, TEXT('a'), TEXT('z'));
	fsm->AddRule(code2, code2, TEXT('a'), TEXT('z'));

	auto w = fsm->CreateWalker();

	bool readed = w->VerifyLiteral(TEXT("valid@mail.com"));
	Assert::IsTrue(readed);
	Assert::AreEqual(w->GetContext(), Base::Fsm::FINITE);

	w->Reset();
	readed = w->VerifyLiteral(TEXT("invalid@readed"));
	Assert::IsTrue(readed);
	Assert::AreEqual(w->GetContext(), Base::Fsm::NON_FINITE);

	w->Reset();
	readed = w->VerifyLiteral(TEXT("invalid.not.readed"));
	Assert::IsFalse(readed);
}


};
}