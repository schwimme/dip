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

enum TOKEN
{
	SPACE,
	CLASS,
	ID,
	CLASS_ID,
	FUNC_ID,
	COLON,
	OPEN,
	CLOSE,
	VISIBILITY,
	NEW_LINE,
	TAB,

	CLASS_IMPL,
	FUNC
};

struct la_builder_fake:
	checklib::la_cfg_builder_intf
{
	virtual std::shared_ptr<checklib::la_cfg> build(const sys::string& path) const override
	{
		auto cfg = std::make_shared<checklib::la_cfg>();

		cfg->m_tokens =
		{
			{ CLASS,      TEXT("class") },
			{ CLASS,      TEXT("struct") },
			{ ID,         TEXT("([A-Z])+") },
			{ CLASS_ID,   TEXT("C([A-Z])+") },
			{ SPACE,      TEXT(" ") },
			{ NEW_LINE,   TEXT("\n") },
			{ TAB,        TEXT("\t") },
			{ OPEN,       TEXT("{") },
			{ CLOSE,      TEXT("};") },
			{ FUNC_ID,    TEXT("([A-Z])+\\(\\);\n")},
			{ VISIBILITY, TEXT("public:\n") },
			{ VISIBILITY, TEXT("protected:\n") },
			{ VISIBILITY, TEXT("private:\n") },
		};

		cfg->m_priorityGroups =
		{
			{CLASS_ID, ID}
		};

		return cfg;
	}
};

struct sa_builder_fake:
	checklib::sa_cfg_builder_intf
{
	virtual std::shared_ptr<checklib::sa_cfg> build(const sys::string& path) const override
	{
		auto cfg = std::make_shared<checklib::sa_cfg>();

		cfg->m_rules =
		{
			{ CLASS,{},{ SPACE, CLASS_ID, NEW_LINE, OPEN, NEW_LINE, CLASS_IMPL, NEW_LINE, CLOSE } },
			
			{ SPACE, {SPACE}, {} },
			{ CLASS_ID,{ CLASS_ID },{} },
			{ NEW_LINE,{ NEW_LINE },{} },
			{ OPEN,{ OPEN },{} },
			{ CLOSE,{ CLASS_IMPL, CLOSE },{} },
			{ VISIBILITY, {CLASS_IMPL}, { CLASS_IMPL }},
			{ TAB,{ CLASS_IMPL },{ FUNC, CLASS_IMPL }},
			{ ID, {FUNC}, {SPACE, FUNC_ID}}
		};

		return cfg;
	}
};

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
	virtual std::shared_ptr<checklib::la_cfg_builder_intf> create_la_cfg_builder() const override
	{
		return std::make_shared<la_builder_fake>();
	}

	virtual std::shared_ptr<checklib::sa_cfg_builder_intf> create_sa_cfg_builder() const override
	{
		return std::make_shared<sa_builder_fake>();
	}

	virtual std::shared_ptr<checklib::worker_intf> create_worker(std::shared_ptr<base::fsm_walker_intf> spFsmWalker, std::shared_ptr<base::pda_walker_intf> spPdaWalker, accident_handler& handler) const override
	{
		return std::make_shared<checker_worker_fake>(spFsmWalker, spPdaWalker, handler);
	}
};

TEST_METHOD(KTTODO_ALL_UT)
{
	test_checker c;
	c.configure(TEXT(""), TEXT(""));

	c.check({ TEXT("") });
}

};
}