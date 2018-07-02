// example.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <ccc/ccc_impl.h>

#include <crossmodule/adapters/vector.h>
#include <thread>
#include <chrono>
#include "ccc/checking_module/impl.h"
#include <iostream>
#include "checker/checker.h"

int g_files_count = 0;
void wait_for_processed_files(int num)
{
	while (g_files_count != num);

	g_files_count = 0;
}


struct accident_handler: checklib::incident_handler_intf
{
	sys::string identify_accident(checklib::incident_handler_intf::incident_type t)
	{
		switch (t)
		{
		case checklib::incident_handler_intf::incident_type::unrecognized_token: return "unrecognized token";
		case checklib::incident_handler_intf::incident_type::no_rule:return "no rule";
		case checklib::incident_handler_intf::incident_type::unexpected_end_of_file: return "unexpected_end_of_file";
		default: return "unknown";
		}
	}


	enum class incident_type
	{
		unrecognized_token,
		no_rule,
		unexpected_end_of_file
	};

	virtual void on_incident(incident_info info, action* action_to_take, cross::settable_string_ref* replacement) noexcept override 
	{
		std::cout << 
			TEXT("[") + sys::string(info.file.m_data, info.file.m_size) +  " " + sys::to_string(info.line) +  ":" + sys::to_string(info.col) +  "] - incident[" + identify_accident(info.type) + "]\n";
	}

	virtual void on_finish(cross::string_ref file, error_t err) override
	{
		++g_files_count;
	}
};


void example_checker_lex(const std::string& good, const std::string& bad)
{
	accident_handler h;
	checklib::checker_impl l;
	
	sys::string la = TEXT(R"regex({[{1,"+(?(-(az)-(AZ)_))*(::+(?(-(az)-(AZ)_)))"}],[[1]]})regex");
	sys::string sa = TEXT("[{1,[],[]}]");
	l.configure(h, la, sa);

	l.check({ good, bad });
	wait_for_processed_files(2);
}


void example_checker_syn(const std::string& good, const std::string& bad)
{
	accident_handler h;
	checklib::checker_impl l;

	sys::string la = TEXT(R"regex({[{1,"+(?(-(az)-(AZ)_))"},{2,"\("},{3,"\)"}],[[1,2,3]]})regex");
	sys::string sa = TEXT("[{1,[3],[1,3]},{2,[],[3]},{3,[1,3],[1]},{3,[1,3],[]}]");
	l.configure(h, la, sa);

	l.check({ good, bad });
	wait_for_processed_files(2);
}


void example_ccc(const std::string& good, const std::string& bad)
{
	accident_handler h;
	ccc::checking_module_impl l;

	l.configure("", h);

	l.check_files({ good, bad });

	wait_for_processed_files(2);
}


int main(int argc, char*argv[])
{
//	example_checker_lex(argv[1], argv[2]);
//	example_checker_syn(argv[3], argv[4]);
	example_ccc(argv[5], argv[6]);
}

