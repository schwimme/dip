#include "stdafx.h"
#include "CppUnitTest.h"

#include <base/FiniteStateMachine/fsm.h>

#include <crossmodule/types/string.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace base_test
{		

TEST_CLASS(fsm_test)
{
public:
	std::shared_ptr<base::fsm_impl> create_fsm(const sys::string& regex)
	{
		std::shared_ptr<base::fsm_context_factory_intf> f = std::make_shared<base::fsm::base_context_factory>();
		std::shared_ptr<base::fsm_impl> fsm = std::make_shared<base::fsm_impl>(f);
		base::fsm::state_id a = fsm->generate_state(0);
		fsm->set_start(a);

		fsm->add_regex(a, cross::string_ref(regex.c_str(), regex.size()), base::fsm::VALID, base::fsm::INVALID);

		return fsm;
	}

TEST_METHOD(simple_example)
{
	auto fsm = create_fsm(TEXT("abc"));
	std::shared_ptr<base::fsm_walker_intf> w = fsm->create_walker();

	Assert::IsTrue(w->verify_literal(TEXT("abc")));
	Assert::IsTrue(w->get_context() == base::fsm::VALID);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("ab")));
	Assert::IsTrue(w->get_context() == base::fsm::INVALID);
	w->reset();

	Assert::IsFalse(w->verify_literal(TEXT("ac")));
}


TEST_METHOD(simple_iteration)
{
	auto fsm = create_fsm(TEXT("*(abc)"));
	std::shared_ptr<base::fsm_walker_intf> w = fsm->create_walker();

	Assert::IsTrue(w->verify_literal(TEXT("")));
	Assert::IsTrue(w->get_context() == base::fsm::VALID);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("abcabc")));
	Assert::IsTrue(w->get_context() == base::fsm::VALID);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("ab")));
	Assert::IsTrue(w->get_context() == base::fsm::INVALID);
	w->reset();

	Assert::IsFalse(w->verify_literal(TEXT("ac")));
}


TEST_METHOD(simple_iteration_positive)
{
	auto fsm = create_fsm(TEXT("+(abc)"));
	std::shared_ptr<base::fsm_walker_intf> w = fsm->create_walker();

	Assert::IsTrue(w->verify_literal(TEXT("")));
	Assert::IsTrue(w->get_context() == base::fsm::INVALID);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("abcabc")));
	Assert::IsTrue(w->get_context() == base::fsm::VALID);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("ab")));
	Assert::IsTrue(w->get_context() == base::fsm::INVALID);
	w->reset();

	Assert::IsFalse(w->verify_literal(TEXT("ac")));
}


TEST_METHOD(simple_range)
{
	auto fsm = create_fsm(TEXT("-(az)-(AZ)"));
	std::shared_ptr<base::fsm_walker_intf> w = fsm->create_walker();

	Assert::IsTrue(w->verify_literal(TEXT("aC")));
	Assert::IsTrue(w->get_context() == base::fsm::VALID);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("hY")));
	Assert::IsTrue(w->get_context() == base::fsm::VALID);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("b")));
	Assert::IsTrue(w->get_context() == base::fsm::INVALID);
	w->reset();

	Assert::IsFalse(w->verify_literal(TEXT("hh")));
}


TEST_METHOD(simple_selection)
{
	auto fsm = create_fsm(TEXT("?(aA)?(bBcC)"));
	std::shared_ptr<base::fsm_walker_intf> w = fsm->create_walker();

	Assert::IsTrue(w->verify_literal(TEXT("aC")));
	Assert::IsTrue(w->get_context() == base::fsm::VALID);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("Ab")));
	Assert::IsTrue(w->get_context() == base::fsm::VALID);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("A")));
	Assert::IsTrue(w->get_context() == base::fsm::INVALID);
	w->reset();

	Assert::IsFalse(w->verify_literal(TEXT("bC")));
}


TEST_METHOD(mail_validation)
{
	auto fsm = create_fsm(TEXT("+(-(az))@+(-(az))\\.?(.(cz).(com))"));
	std::shared_ptr<base::fsm_walker_intf> w = fsm->create_walker();

	Assert::IsTrue(w->verify_literal(TEXT("adam")));
	Assert::IsTrue(w->get_context() == base::fsm::INVALID);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("adam@troska")));
	Assert::IsTrue(w->get_context() == base::fsm::INVALID);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("adam@troska.cz")));
	Assert::IsTrue(w->get_context() == base::fsm::VALID);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("adam@troska.com")));
	Assert::IsTrue(w->get_context() == base::fsm::VALID);
	w->reset();

	Assert::IsFalse(w->verify_literal(TEXT("adam@troska.info")));

	Assert::IsFalse(w->verify_literal(TEXT("Adam@Troska.Cz")));
}


};

}
