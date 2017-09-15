#include "fsm.h"

#include "fsmwalker.h"

#include <base/Debugging/debug.h>
#include <base/Algorithm/algorithm.h>


namespace Base
{


CFsm::CFsm(std::shared_ptr<IFsmContextFactory>& spCtxFactory):
	m_spContextFactory(spCtxFactory)
{}


void CFsm::Initialize()
{
}


void CFsm::AddRule(const Fsm::StateId& from, const Fsm::StateId& to)
{
	VERIFY(m_optimized == false);
	VERIFY(m_states.find(from) != m_states.end());
	VERIFY(m_states.find(to) != m_states.end());

	if (from != to)
	{
		m_states[from]->AddRule(to, Fsm::detail::EPSILON);
	}
}


void CFsm::AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Fsm::AlphabetType ch)
{
	VERIFY(m_optimized == false);
	VERIFY(ch != Fsm::detail::EPSILON);
	VERIFY(m_states.find(from) != m_states.end());
	VERIFY(m_states.find(to) != m_states.end());

	m_states[from]->AddRule(to, ch);
}


void CFsm::AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Fsm::AlphabetType a, Fsm::AlphabetType b)
{
	// KTTODO - optimize this using ranges (should be done after UT).
	do
	{
		AddRule(from, to, a);
	} while (a++ != b);
}


void CFsm::SetStart(const Fsm::StateId& state)
{
	VERIFY(m_optimized == false);
	VERIFY(m_start.empty());

	m_start = state;
}


Fsm::StateId CFsm::GenerateState(Fsm::ContextId ctx)
{
	VERIFY(m_optimized == false);
	Fsm::StateId s = Fsm::CStateIdGenerator::Generate();
	m_states[s] = std::make_unique<Fsm::CState>(ctx);
	return s;
}


void CFsm::Optimize()
{
	m_optimized = true;

	// Remove epsilon rules:
	for (auto& s : m_states)
	{
		Fsm::CState::Holder& pState = s.second;
		const std::vector<Fsm::StateId>& epsilonRules = pState->GetRules(Fsm::detail::EPSILON);
	
		for (const auto& r : epsilonRules)
		{
			pState->AddOptimizedEpsilonRule(*(m_states[r]));
		}

		pState->RemoveRules(Fsm::detail::EPSILON);
	}

	// Remove unreachable states:
	std::vector<Fsm::StateId> reachableStates = { m_start };
	size_t position = 0;
	do
	{
		const auto& allStateRules = m_states[reachableStates[position]]->GetAllRules();
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


const std::vector<Fsm::StateId>& CFsm::GetNextStates(const Fsm::StateId& currentState, Fsm::AlphabetType ch) const
{
	auto it = m_states.find(currentState);
	VERIFY(it != m_states.end());

	return it->second->GetRules(ch);
}


Fsm::ContextId CFsm::GetContext(const Fsm::StateId& state) const
{
	auto it = m_states.find(state);
	VERIFY(it != m_states.end());

	return it->second->GetContext();
}


const Fsm::StateId& CFsm::GetStart() const
{
	return m_start;
}


std::shared_ptr<IFsmWalker> CFsm::CreateWalker()
{
	if (m_optimized == false)
	{
		Optimize();
	}

	return std::make_shared<CFsmWalker>(*this, m_spContextFactory);
}



}
