#include "fsm.h"

#include "fsmwalker.h"

#include <base/Debugging/debug.h>
#include <base_intf/Algorithm/algorithm.h>
#include "regexcontextfactory.h"
#include <types/string.h>
#include <crossmodule/adapters/vector.h>

namespace base
{


fsm_impl::fsm_impl(std::shared_ptr<fsm_context_factory_intf>& spCtxFactory):
	m_spContextFactory(spCtxFactory),
	m_optimized(false)
{}


void fsm_impl::initialize()
{
	// Currently no action taken.
}


void fsm_impl::add_rule(const fsm::state_id& from, const fsm::state_id& to)
{
	ASSERT_NO_EVAL(m_optimized == false);
	ASSERT_NO_EVAL(m_states.find(from) != m_states.end());
	ASSERT_NO_EVAL(m_states.find(to) != m_states.end());

	if (from != to)
	{
		m_states[from]->add_rule(to, fsm::detail::EPSILON);
	}
}


void fsm_impl::add_rule(const fsm::state_id& from, const fsm::state_id& to, base::char_t ch)
{
	ASSERT_NO_EVAL(m_optimized == false);
	ASSERT_NO_EVAL(ch != Fsm::detail::EPSILON);
	ASSERT_NO_EVAL(m_states.find(from) != m_states.end());
	ASSERT_NO_EVAL(m_states.find(to) != m_states.end());

	m_states[from]->add_rule(to, ch);
}


void fsm_impl::add_regex(const fsm::state_id& from, crossmodule::string_ref regex, fsm::context_id valid, fsm::context_id invalid)
{
	base::string regex_non_crossmodule(regex.m_data, regex.m_size);
	std::shared_ptr<fsm_context_factory_intf> spCtxFactory = std::make_shared<fsm::regex_context_factory>(valid, invalid);
	fsm_impl regexFsm(spCtxFactory);

	// KTTODO use intf and injection
	fsm::builder b;
	b.build_fsm_from_regex(regexFsm, regex_non_crossmodule, valid, invalid);

	// Get fsm content:
	for (auto& i : regexFsm.m_states)
	{
		m_states[i.first] = std::move(i.second);
	}

	// Add rule 'from'->epsilon->fsm.start
	add_rule(from, regexFsm.m_start);
}


void fsm_impl::add_rule(const fsm::state_id& from, const fsm::state_id& to, base::char_t a, base::char_t b)
{
	// KTTODO - optimize this using ranges (should be done after UT to be sure, optimization doesnt break anything).
	do
	{
		add_rule(from, to, a);
	} while (a++ != b);
}


void fsm_impl::set_start(const fsm::state_id& state)
{
	ASSERT_NO_EVAL(m_optimized == false);
	ASSERT_NO_EVAL(m_start.empty());

	m_start = state;
}


fsm::state_id fsm_impl::generate_state(fsm::context_id ctx)
{
	ASSERT(m_optimized == false);
	fsm::state_id s = fsm::state_id_generator::generate();
	m_states[s] = std::make_unique<fsm::state>(ctx);
	return s;
}


void fsm_impl::optimize(optimization_level_t level)
{
	if (level > optimization_level_t::NOTHING)
	{
		remove_epsilon_rules();
	}

	if (level > optimization_level_t::EPSILON_RULES)
	{
		remove_unreachable_states();
	}

	// After this call, fsm has no longer some exported states, so cant be changed:
	m_optimized = true;

	if (level > optimization_level_t::UNREACHABLE_STATES)
	{
		determine();
	}

	if (level > optimization_level_t::DETERMINE)
	{
		minimize();
	}
}


bool fsm_impl::remove_epsilon_rules_impl(fsm::state::holder& pState)
{
	if (pState->m_epsVisited)
	{
		return false;
	}

	pState->m_epsVisited = true;

	const std::vector<fsm::state_id>& epsilonRules = pState->get_rules(fsm::detail::EPSILON);
	for (const auto& r : epsilonRules)
	{
		remove_epsilon_rules_impl(m_states[r]);
		pState->add_optimized_epsilon_rule(*(m_states[r]));
		pState->m_context = m_spContextFactory->select_context(&crossmodule::std_vector_on_enumerator<fsm::context_id>(std::vector<fsm::context_id>{ pState->m_context, m_states[r]->m_context }));
	}

	pState->remove_rules(fsm::detail::EPSILON);

	return true;
}


void fsm_impl::remove_epsilon_rules()
{
	for (auto& s : m_states)
	{
		fsm::state::holder& pState = s.second;
		remove_epsilon_rules_impl(pState);
	}
}


void fsm_impl::remove_unreachable_states()
{
	std::vector<fsm::state_id> reachableStates = { m_start };
	size_t position = 0;
	do
	{
		const auto& allStateRules = m_states[reachableStates[position]]->m_rules;
		std::vector<fsm::state_id> allNextStates;
		for (const auto& p : allStateRules)
		{
			allNextStates = base::make_union(allNextStates, p.second);
		}

		for (auto& i : allNextStates)
		{
			if (base::find(reachableStates, i) == false)
			{
				reachableStates.push_back(std::move(i));
			}
		}

		++position;

	} while (position != reachableStates.size());

	// Commit point:
	std::unordered_map<fsm::state_id, fsm::state::holder> newStates;
	for (const auto& s : reachableStates)
	{
		newStates[s] = std::move(m_states[s]);
	}

	m_states = std::move(newStates);
}


void fsm_impl::determine()
{
	// KTTODO
}


void fsm_impl::minimize()
{
	// KTTODO
}


const std::vector<fsm::state_id>& fsm_impl::get_next_states(const fsm::state_id& currentState, base::char_t ch) const
{
	auto it = m_states.find(currentState);
	ASSERT(it != m_states.end());

	return it->second->get_rules(ch);
}


fsm::context_id fsm_impl::get_context(const fsm::state_id& state) const
{
	auto it = m_states.find(state);
	ASSERT(it != m_states.end());

	return it->second->m_context;
}


const fsm::state_id& fsm_impl::get_start() const
{
	return m_start;
}


std::shared_ptr<fsm_walker_intf> fsm_impl::create_walker()
{
	if (m_optimized == false)
	{
		// Cannot determine fsm, because may context lose here.
		optimize(optimization_level_t::UNREACHABLE_STATES);
	}

	return std::make_shared<fsm::walker>(*this, m_spContextFactory);
}


base::string fsm_impl::dump() const
{
	base::string out;

	// KTTODO
//	// For all states:
//	for (const auto& state : m_states)
//	{
//		const base::string& beginState = (state.first + TEXT('(') + base::to_string(get_context(state.first)) + TEXT(')'));
//
//		// For all characters in rule:
//		for (const auto& character : state.second->m_rules)
//		{
//			// For all destination states:
//			for (const auto& destState : character.second)
//			{
//				out += beginState;
//				out += TEXT("--");
//				out += ((character.first == fsm::detail::EPSILON) ? TEXT('_') : character.first);
//				out += TEXT("->");
//				out += (destState + TEXT('(') + base::to_string(get_context(destState)) + TEXT(')'));
//				out += TEXT('\n');
//			}
//		}
//	}

	return out;
}


}
