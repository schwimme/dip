#pragma once


#include "fsm_intf.h"
#include "stateidgenerator.h"
#include "state.h"
#include "walkable_intf.h"
#include <unordered_map>
#include "basecontextfactory.h"


namespace Base
{


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

public:
	//! \copydoc IFsm::AddRule(const Fsm::StateId& from, const Fsm::StateId& to)
	virtual void AddRule(const Fsm::StateId& from, const Fsm::StateId& to) override;

	//! \copydoc IFsm::AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Fsm::AlphabetType& ch)
	virtual void AddRule(const Fsm::StateId& from, const Fsm::StateId& to, Fsm::AlphabetType ch) override;

	//! \copydoc IFsm::SetIdle
	virtual void SetStart(const Fsm::StateId& state) override;

	//! \copydoc IFsm::GenerateStateId
	virtual Fsm::StateId GenerateState(Fsm::ContextId ctx) override;

	virtual void Optimize() override;

	//! \copydoc IFsm::CreateWalker
	virtual std::shared_ptr<IFsmWalker> CreateWalker() override;

public:
	virtual const std::vector<Fsm::StateId>& GetNextStates(const Fsm::StateId& currentState, Fsm::AlphabetType ch) const override;
	
	virtual Fsm::ContextId GetContext(const Fsm::StateId& state) const override;

	virtual const Fsm::StateId& GetStart() const override;

private:
	void Initialize();

private:
	bool m_optimized = false;
	std::unordered_map<Fsm::StateId, Fsm::CState::Holder> m_states;
	Fsm::StateId m_start;
	std::shared_ptr<IFsmContextFactory> m_spContextFactory;
};


}