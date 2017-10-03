#include <base/FiniteStateMachine/fsm.h>

#include <iostream>


#define TEST_ASSERT(c) \
if ((c) == false) \
{\
	std::cout << __LINE__ << " Assert failed - (" << #c << ")\n"; throw 0;\
}\


//	Fsm representing 'a(a+b)*' regex.
void SimpleFsm()
{
	// Create FSM:
	std::shared_ptr<Base::IFsm> spFsm = Base::CFsm::Factory().CreateFiniteStateMachine();

	// Generate states:
	Base::Fsm::StateId s1 = spFsm->GenerateState(0);
	Base::Fsm::StateId s2 = spFsm->GenerateState(1);
	spFsm->SetStart(s1);

	// Generate rules:
	spFsm->AddRule(s1, s2, TEXT('a'));
	spFsm->AddRule(s2, s2, TEXT('a'));
	spFsm->AddRule(s2, s2, TEXT('b'));

	// Tests:
	std::shared_ptr<Base::IFsmWalker> spWalker = spFsm->CreateWalker();
	bool readed = false;

	// Not completly readed literal:
	readed = spWalker->VerifyLiteral(TEXT("acb"));
	TEST_ASSERT(readed == false);
	spWalker->Reset();

	// Readed literal - invalid:
	readed = spWalker->VerifyLiteral(TEXT(""));
	TEST_ASSERT(readed == true);
	TEST_ASSERT(spWalker->GetContext() == 0);
	spWalker->Reset();

	// Readed literal - valid:
	readed = spWalker->VerifyLiteral(TEXT("abaabb"));
	TEST_ASSERT(readed == true);
	TEST_ASSERT(spWalker->GetContext() == 1);
}

// Same example as SimpleFsm but with automatic building of 'a(a+b)*' regex.
void SimpleFsmUsingRegex()
{
	// Create FSM:
	std::shared_ptr<Base::IFsm> spFsm = Base::CFsm::Factory().CreateFiniteStateMachine();

	// Generate initial state:
	Base::Fsm::StateId s1 = spFsm->GenerateState(0);
	spFsm->SetStart(s1);
	spFsm->AddRegex(s1, TEXT("a([a-b])*"), 1, 0);

	// Tests:
	std::shared_ptr<Base::IFsmWalker> spWalker = spFsm->CreateWalker();
	bool readed = false;

	// Not completly readed literal:
	readed = spWalker->VerifyLiteral(TEXT("acb"));
	TEST_ASSERT(readed == false);
	spWalker->Reset();

	// Readed literal - invalid:
	readed = spWalker->VerifyLiteral(TEXT(""));
	TEST_ASSERT(readed == true);
	TEST_ASSERT(spWalker->GetContext() == 0);
	spWalker->Reset();

	// Readed literal - valid:
	readed = spWalker->VerifyLiteral(TEXT("abaabb"));
	TEST_ASSERT(readed == true);
	TEST_ASSERT(spWalker->GetContext() == 1);

}

// Decision if given literal is simple id '[a-z]+' or member 'm[b-z]*'
// Note - members cannot has 'a' chars to show that id starting with m is not always just members but can be simple identificator.
// Need specific ctx factory:
struct MemberCtxFactory:
	public Base::IFsmContextFactory
{
	static const Base::Fsm::ContextId INVALID;
	static const Base::Fsm::ContextId IDENTIFICATOR;
	static const Base::Fsm::ContextId MEMBER;

	virtual Base::Fsm::ContextId SelectContext(const std::vector<Base::Fsm::ContextId>& allContexts) const
	{
		if (Base::Find(allContexts, MEMBER))
		{
			return MEMBER;
		}

		if (Base::Find(allContexts, IDENTIFICATOR))
		{
			return IDENTIFICATOR;
		}

		return INVALID;
	}
};

const Base::Fsm::ContextId MemberCtxFactory::INVALID = 0;
const Base::Fsm::ContextId MemberCtxFactory::IDENTIFICATOR = 1;
const Base::Fsm::ContextId MemberCtxFactory::MEMBER = 2;

void IdOrMemberDecission()
{
	std::shared_ptr<Base::IFsmContextFactory> spCtxFactory = std::make_shared<MemberCtxFactory>();
	
	// Create FSM with factory:
	std::shared_ptr<Base::IFsm> spFsm = Base::CFsm::Factory().CreateFiniteStateMachine(spCtxFactory);

	// Generate initial state:
	Base::Fsm::StateId s1 = spFsm->GenerateState(MemberCtxFactory::INVALID);
	spFsm->SetStart(s1);

	// Create ID rule:
	spFsm->AddRegex(s1, TEXT("([a-z])+"), MemberCtxFactory::IDENTIFICATOR, MemberCtxFactory::INVALID);

	// Create member rule:
	spFsm->AddRegex(s1, TEXT("m([b-z])*"), MemberCtxFactory::MEMBER, MemberCtxFactory::INVALID);

	// Tests:
	std::shared_ptr<Base::IFsmWalker> spWalker = spFsm->CreateWalker();
	bool readed = false;

	// Not completly readed literal:
	readed = spWalker->VerifyLiteral(TEXT("ab1"));
	TEST_ASSERT(readed == false);
	spWalker->Reset();

	// Identificator:
	readed = spWalker->VerifyLiteral(TEXT("identificator"));
	TEST_ASSERT(readed == true);
	TEST_ASSERT(spWalker->GetContext() == MemberCtxFactory::IDENTIFICATOR);
	spWalker->Reset();

	// Identificator with 'm' on start:
	readed = spWalker->VerifyLiteral(TEXT("midentificator"));
	TEST_ASSERT(readed == true);
	TEST_ASSERT(spWalker->GetContext() == MemberCtxFactory::IDENTIFICATOR);
	spWalker->Reset();

	// Member:
	readed = spWalker->VerifyLiteral(TEXT("member"));
	TEST_ASSERT(readed == true);
	TEST_ASSERT(spWalker->GetContext() == MemberCtxFactory::MEMBER);
	spWalker->Reset();
}

int main()
{
	std::cout << "Example app start:\n";	

	SimpleFsm();
	SimpleFsmUsingRegex();

	std::cout << "Example app end:\n";
}
