#pragma once


#include "fsm_intf.h"
#include "stateidgenerator.h"
#include "state.h"
#include "walkable_intf.h"
#include <unordered_map>
#include "basecontextfactory.h"
#include "fsmbuilder.h"


namespace Base
{


//! \brief	Fsm main class.
class CFsm:
	public IFsm,
	public Fsm::IWalkable
{
public:
	struct Factory
	{
		virtual std::shared_ptr<IFsm> CreateFiniteStateMachine(std::shared_ptr<IFsmContextFactory> spCtxFactory = std::make_shared<Fsm::CBaseContextFactory>()) const
		{
			std::shared_ptr<CFsm> spImpl = std::make_shared<CFsm>(spCtxFactory);
			spImpl->Initialize();
			return spImpl;
		}
	};

	CFsm(std::shared_ptr<IFsmContextFactory>& spCtxFactory);

private:
	using StatesStorage = std::unordered_map<Fsm::StateId, Fsm::CState::Holder>;

public:
	//! \copydoc IFsm::AddRule(const Fsm::StateId& from, const Fsm::StateId& to)
	virtual void AddRule(const Fsm::StateId& from, const Fsm::StateId& to) override;

	//! \copydoc IFsm::AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Fsm::AlphabetType& ch)
	virtual void AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Fsm::AlphabetType ch) override;

	//! \copydoc IFsm::AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Fsm::AlphabetType& a, Fsm::AlphabetType b)
	virtual void CFsm::AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Fsm::AlphabetType a, Fsm::AlphabetType b) override;

	//! \copydoc IFsm::SetIdle
	virtual void SetStart(const Fsm::StateId& state) override;

	//! \copydoc IFsm::GenerateStateId
	virtual Fsm::StateId GenerateState(Fsm::ContextId ctx) override;

	//! \copydoc IFsm::CreateWalker
	virtual std::shared_ptr<IFsmWalker> CreateWalker() override;

public:
	//! \copydoc Fsm::IWalkable::GetNextStates
	virtual const std::vector<Fsm::StateId>& GetNextStates(const Fsm::StateId& currentState, Fsm::AlphabetType ch) const override;
	
	//! \copydoc Fsm::IWalkable::GetContext
	virtual Fsm::ContextId GetContext(const Fsm::StateId& state) const override;

	//! \copydoc Fsm::IWalkable::GetStart
	virtual const Fsm::StateId& GetStart() const override;

private:
	void Initialize();
	void Optimize();

private:
	StatesStorage						m_states;			// All states in fsm.
	std::shared_ptr<IFsmContextFactory>	m_spContextFactory;	// Injected ctx factory.

	bool			m_optimized;	// After optimization fsm became readonly. KTTODO - Use local optimized copy to remove readonly property.
	Fsm::StateId	m_start;		// Id of start state.

	friend class Fsm::CBuilder;
};


}
