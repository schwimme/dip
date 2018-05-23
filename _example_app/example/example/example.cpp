// example.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <ccc/ccc_impl.h>
#include <ccc_intf/ccc_checking_module_intf.h>

#include <crossmodule/adapters/vector.h>

struct accident_handler: checklib::incident_handler_intf
{
	virtual void on_incident(incident_info info, action* action_to_take, cross::settable_string_ref* replacement) noexcept override {}
	virtual void on_finish(cross::string_ref file, error_t err) override {}
};

int main(int argc, char**argv)
{
	std::vector<cross::string_ref> a;
	while (*argv)
	{
		a.push_back(cross::string_ref(*argv, strlen(*argv)));
		(*argv)++;
	}

	ccc::ccc_impl impl;

	ccc::checking_module_intf* pModule;
	impl.get_checking_module(pModule);
	accident_handler h;

	pModule->configure("", &h);
	
	cross::std_vector_on_enumerator<cross::string_ref> adapter(a);
	pModule->check_files(&adapter);

    return 0;
}

