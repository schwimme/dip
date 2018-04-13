#include "stdafx.h"
#include "CppUnitTest.h"

#include "../checker.h"
#include <sys/types/string.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Checker_test
{		

TEST_CLASS(LA_test)
{
public:

struct checker_worker_fake:
	checklib::worker
{
	checker_worker_fake(std::shared_ptr<base::fsm_walker_intf> spFsmWalker, std::shared_ptr<base::pda_walker_intf> spPdaWalker, checklib::accident_handler& handler) :
		checklib::worker(spFsmWalker, spPdaWalker, handler)
	{}

	virtual sys::string read_file_content() const override
	{
		return TEXT("class MANAGER\n{\nprotected:\n\tABC FUNC();\n};");
	}
};

struct test_checker :
	public checklib::checker_impl
{
	virtual std::shared_ptr<checklib::worker_intf> create_worker(std::shared_ptr<base::fsm_walker_intf> spFsmWalker, std::shared_ptr<base::pda_walker_intf> spPdaWalker, accident_handler& handler) const override
	{
		return std::make_shared<checker_worker_fake>(spFsmWalker, spPdaWalker, handler);
	}
};

TEST_METHOD(KTTODO_ALL_UT)
{
	sys::string la = TEXT(R"({[{1,"r1"},{2,"r2"}],[[1,2],[3,4,5,6]]})");
	sys::string sa = TEXT(R"([{0,[26,9],[]},{5,[],[1243]}])");

	test_checker c;
	c.configure(la, sa);

	c.check({ TEXT("") });
}

};
}