#include "fsm.h"

#include "fsmwalker.h"

#include <base/Debugging/debug.h>
#include <base/Algorithm/algorithm.h>


namespace Base
{


CFsm::CFsm(std::shared_ptr<IFsmContextFactory>& spCtxFactory):
	m_spContextFactory(spCtxFactory),
	m_optimized(false)
{}


void CFsm::Initialize()
{
	// Currently no action taken.
}


void CFsm::AddRule(const Fsm::StateId& from, const Fsm::StateId& to)
{
	ASSERT_NO_EVAL(m_optimized == false);
	ASSERT_NO_EVAL(m_states.find(from) != m_states.end());
	ASSERT_NO_EVAL(m_states.find(to) != m_states.end());

	if (from != to)
	{
		m_states[from]->AddRule(to, Fsm::detail::EPSILON);
	}
}


void CFsm::AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Fsm::AlphabetType ch)
{
	ASSERT_NO_EVAL(m_optimized == false);
	ASSERT_NO_EVAL(ch != Fsm::detail::EPSILON);
	ASSERT_NO_EVAL(m_states.find(from) != m_states.end());
	ASSERT_NO_EVAL(m_states.find(to) != m_states.end());

	m_states[from]->AddRule(to, ch);
}


void CFsm::AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Fsm::AlphabetType a, Fsm::AlphabetType b)
{
	// KTTODO - optimize this using ranges (should be done after UT to be sure, optimization doesnt break anything).
	do
	{
		AddRule(from, to, a);
	} while (a++ != b);
}


void CFsm::SetStart(const Fsm::StateId& state)
{
	ASSERT_NO_EVAL(m_optimized == false);
	ASSERT_NO_EVAL(m_start.empty());

	m_start = state;
}


Fsm::StateId CFsm::GenerateState(Fsm::ContextId ctx)
{
	ASSERT(m_optimized == false);
	Fsm::StateId s = Fsm::CStateIdGenerator::Generate();
	m_states[s] = std::make_unique<Fsm::CState>(ctx);
	return s;
}


void CFsm::Optimize(OptimizationLevel level)
{
	if (level > OptimizationLevel::NOTHING)
	{
		RemoveEpsilonRules();
	}

	if (level > OptimizationLevel::EPSILON_RULES)
	{
		RemoveUnreachableStates();
	}

	// After this call, fsm has no longer some exported states, so cant be changed:
	m_optimized = true;

	if (level > OptimizationLevel::UNREACHABLE_STATES)
	{
		Determine();
	}

	if (level > OptimizationLevel::DETERMINE)
	{
		Minimize();
	}
}


bool CFsm::RemoveEpsilonRulesImpl(Fsm::CState::Holder& pState)
{
	if (pState->m_epsVisited)
	{
		return false;
	}

	pState->m_epsVisited = true;

	const std::vector<Fsm::StateId>& epsilonRules = pState->GetRules(Fsm::detail::EPSILON);
	for (const auto& r : epsilonRules)
	{
		RemoveEpsilonRulesImpl(m_states[r]);
		pState->AddOptimizedEpsilonRule(*(m_states[r]));
		pState->m_context = m_spContextFactory->SelectContext({ pState->m_context, m_states[r]->m_context });
	}

	pState->RemoveRules(Fsm::detail::EPSILON);

	return true;
}


void CFsm::RemoveEpsilonRules()
{
	for (auto& s : m_states)
	{
		Fsm::CState::Holder& pState = s.second;
		RemoveEpsilonRulesImpl(pState);
	}
}


void CFsm::RemoveUnreachableStates()
{
	std::vector<Fsm::StateId> reachableStates = { m_start };
	size_t position = 0;
	do
	{
		const auto& allStateRules = m_states[reachableStates[position]]->m_rules;
		std::vector<Fsm::StateId> allNextStates;
		for (const auto& p : allStateRules)
		{
			allNextStates = Base::Union(allNextStates, p.second);
		}

		for (auto& i : allNextStates)
		{
			if (Base::Find(reachableStates, i) == false)
			{
				reachableStates.push_back(std::move(i));
			}
		}

		++position;

	} while (position != reachableStates.size());

	// Commit point:
	std::unordered_map<Fsm::StateId, Fsm::CState::Holder> newStates;
	for (const auto& s : reachableStates)
	{
		newStates[s] = std::move(m_states[s]);
	}

	m_states = std::move(newStates);
}


void CFsm::Determine()
{
	// KTTODO
}

void CFsm::Minimize()
{
	// KTTODO
}


const std::vector<Fsm::StateId>& CFsm::GetNextStates(const Fsm::StateId& currentState, Fsm::AlphabetType ch) const
{
	auto it = m_states.find(currentState);
	ASSERT(it != m_states.end());

	return it->second->GetRules(ch);
}


Fsm::ContextId CFsm::GetContext(const Fsm::StateId& state) const
{
	auto it = m_states.find(state);
	ASSERT(it != m_states.end());

	return it->second->m_context;
}


const Fsm::StateId& CFsm::GetStart() const
{
	return m_start;
}


std::shared_ptr<IFsmWalker> CFsm::CreateWalker()
{
	if (m_optimized == false)
	{
		// Cannot determine fsm, because may context lose here.
		Optimize(OptimizationLevel::UNREACHABLE_STATES);
	}

	return std::make_shared<Fsm::CWalker>(*this, m_spContextFactory);
}


Base::String CFsm::Dump() const
{
	Base::String out;

	// For all states:
	for (const auto& state : m_states)
	{
		const Base::String& beginState = (state.first + TEXT('(') + Base::ToString(GetContext(state.first)) + TEXT(')'));

		// For all characters in rule:
		for (const auto& character : state.second->m_rules)
		{
			// For all destination states:
			for (const auto& destState : character.second)
			{
				out += beginState;
				out += TEXT("--");
				out += ((character.first == Fsm::detail::EPSILON) ? TEXT('_') : character.first);
				out += TEXT("->");
				out += (destState + TEXT('(') + Base::ToString(GetContext(destState)) + TEXT(')'));
				out += TEXT('\n');
			}
		}
	}

	return out;
}


}
