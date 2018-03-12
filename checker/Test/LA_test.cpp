#include "stdafx.h"
#include "CppUnitTest.h"

#include "../checker.h"
#include <types/string.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Checker_test
{		

TEST_CLASS(LA_test)
{
public:

enum TOKEN
{
	NUM,
	SPACE,
	OP
};

struct la_builder_fake:
	checker::la_cfg_builder_intf
{
	virtual std::shared_ptr<checker::la_cfg> build(const base::string& path) const override
	{
		auto cfg = std::make_shared<checker::la_cfg>();

		cfg->m_tokens =
		{
			{ NUM, TEXT("[1-9]([0-9])*")},
			{ SPACE, TEXT(" ") },
			{ OP, TEXT("+") },
			{ OP, TEXT("-") },
			{ OP, TEXT("*") },
			{ OP, TEXT("/") },
			{ OP, TEXT("=") },
		};

		return cfg;
	}
};

struct sa_builder_fake:
	checker::sa_cfg_builder_intf
{
	virtual std::shared_ptr<checker::sa_cfg> build(const base::string& path) const override
	{
		auto cfg = std::make_shared<checker::sa_cfg>();

		cfg->m_rules =
		{
			{NUM, {}, {SPACE, OP, SPACE, NUM}},
			{NUM, {NUM}, {}},
			{SPACE, {SPACE}, {}},
			{OP, {OP}, {}}
		};

		return cfg;
	}
};

struct checker_worker_fake:
	checker::worker
{
	checker_worker_fake(std::shared_ptr<base::fsm_walker_intf> spFsmWalker, std::shared_ptr<base::pda_walker_intf> spPdaWalker, checker::accident_handler& handler) :
		checker::worker(spFsmWalker, spPdaWalker, handler)
	{}

	virtual base::string read_file_content() const override
	{
		return TEXT("1 + 2 ");
	}
};

struct test_checker :
	public checker::checker_impl
{
	virtual std::shared_ptr<checker::la_cfg_builder_intf> create_la_cfg_builder() const override
	{
		return std::make_shared<la_builder_fake>();
	}

	virtual std::shared_ptr<checker::sa_cfg_builder_intf> create_sa_cfg_builder() const override
	{
		return std::make_shared<sa_builder_fake>();
	}

	virtual std::shared_ptr<checker::worker_intf> create_worker(std::shared_ptr<base::fsm_walker_intf> spFsmWalker, std::shared_ptr<base::pda_walker_intf> spPdaWalker, accident_handler& handler) const override
	{
		return std::make_shared<checker_worker_fake>(spFsmWalker, spPdaWalker, handler);
	}
};

TEST_METHOD(KTTODO_ALL_UT)
{
	test_checker c;
	c.configure(TEXT("2"), TEXT("3"));

	c.check({ TEXT("123") });
}

};
}