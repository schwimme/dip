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

#include <base/ll_validator/ll_validator_impl.h>

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


error_t example_using_base()
{
	base::rule_item_terminal
		begin(1), 
		end(2), 
		semicolon(3),
		read(4),
		write(5),
		id(6),
		assign(7),
		add(8),
		copen(9),
		cclose(10),
		comma(11),
		intt(12);

	base::rule_item_non_terminal
		prog(1),
		st_list(2),
		stat(3),
		it_list(4),
		item(5);

	base::ll_validator_intf_impl v;
	v.add_grammar_rule(prog, { begin, st_list });                            // {1,[t1,n2]}

	v.add_grammar_rule(st_list, { stat, semicolon, st_list });               // {2,[n3,t3,n2]}
	v.add_grammar_rule(st_list, { end });                                    // {2,[t2]}

	v.add_grammar_rule(stat, { read, id });                                  // {3,[t4,t6]}
	v.add_grammar_rule(stat, { write, item });                               // {3,[t5,n5]}
	v.add_grammar_rule(stat, { id, assign, add, copen, item, it_list });     // {3,[t6,t7,t8,t9,n5,n4]}

	v.add_grammar_rule(it_list, { comma, item, it_list });                   // {4,[t11,n5,n4]}
	v.add_grammar_rule(it_list, { cclose });                                 // {4,[t10]}

	v.add_grammar_rule(item, { intt });                                      // {5,[t12]}          
	v.add_grammar_rule(item, { id });                                        // {5,[t6]}

	base::ll_validator_walker_intf* p_walker;

	v.create_walker(p_walker, prog, 0);

	RETURN_FAIL(p_walker->process_step(begin));
	RETURN_FAIL(p_walker->process_step(write));
	RETURN_FAIL(p_walker->process_step(intt));
	RETURN_FAIL(p_walker->process_step(semicolon));
	RETURN_FAIL(p_walker->process_step(end));
	return p_walker->is_accepted() ? E_NO_ERROR : -1;
}

void example_using_checklib()
{
	accident_handler h;
	checklib::checker_impl c;

	sys::string la = TEXT(R"regex({[{1,"begin"},{2,"end"},{3,";"},{4,"read"},{5,"write"},{6,"+(-(az))"},{7,":="},{8,"add"},{9,"\("},{10,"\)"},{11,","},{12,"+(-(09))"},{13," "}],[[1,6],[2,6],[4,6],[5,6],[8,6]]})regex");
	sys::string sa = TEXT("[{1,[t1,n2]},{2,[n3,t3,n2]},{2,[t2]},{3,[t4,t6]},{3,[t5,n5]},{3,[t6,t7,t8,t9,n5,n4]},{4,[t11,n5,n4]},{4,[t10]},{5,[t12]},{5,[t6]}]");
	c.configure(h, la, sa, { 13 });

	c.check({ "D:/temp.lng" });
	wait_for_processed_files(1);
}

void example_using_ccc()
{
	accident_handler h;
	ccc::checking_module_impl c;
	c.configure(TEXT(""), h);

	c.check_files({ "D:/dip_test.cpp" });
	wait_for_processed_files(1);
}

int main(int argc, char*argv[])
{
//	error_t rv = example_using_base();
//	example_using_checklib();
	example_using_ccc();
}

