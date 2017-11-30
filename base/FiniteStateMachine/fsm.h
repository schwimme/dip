#pragma once


#include <base_intf/FiniteStateMachine/fsm_intf.h>
#include "stateidgenerator.h"
#include "state.h"
#include "walkable_intf.h"
#include <unordered_map>
#include "basecontextfactory.h"
#include "fsmbuilder.h"


namespace base
{


//! \brief	Fsm main class.
class fsm_impl:
	public fsm_intf,
	public fsm::walkable_intf
{
private:
	//	Order is used in evaluation so do not shuffle this enum.
	enum class optimization_level_t
	{
		NOTHING = 0,
		EPSILON_RULES,
		UNREACHABLE_STATES,
		DETERMINE,
		MINIMIZE
	};

public:
	struct factory
	{
		virtual std::shared_ptr<fsm_intf> create_finite_state_machine(std::shared_ptr<fsm_context_factory_intf> spCtxFactory = std::make_shared<fsm::base_context_factory>()) const
		{
			std::shared_ptr<fsm_impl> spImpl = std::make_shared<fsm_impl>(spCtxFactory);
			spImpl->initialize();
			return spImpl;
		}
	};

	fsm_impl(std::shared_ptr<fsm_context_factory_intf>& spCtxFactory);

private:
	using states_storage_t = std::unordered_map<fsm::state_id, fsm::state::holder>;

public:
	//! \copydoc IFsm::add_rule(const fsm::state_id& from, const fsm::state_id& to)
	virtual void add_rule(const fsm::state_id& from, const fsm::state_id& to) override;

	//! \copydoc IFsm::add_rule(const fsm::state_id& from, const fsm::state_id& to, base::char_t& ch)
	virtual void add_rule(const fsm::state_id& from, const fsm::state_id& to, base::char_t ch) override;

	//! \copydoc IFsm::add_rule(const fsm::state_id& from, const fsm::state_id& to, base::char_t& a, base::char_t b)
	virtual void add_rule(const fsm::state_id& from, const fsm::state_id& to, base::char_t a, base::char_t b) override;

	//! \copydoc IFsm::add_regex
	virtual void add_regex(const fsm::state_id& from, crossmodule::string_ref regex, fsm::context_id valid, fsm::context_id invalid) override;

	//! \copydoc IFsm::set_start
	virtual void set_start(const fsm::state_id& state) override;

	//! \copydoc IFsm::generate_state
	virtual fsm::state_id generate_state(fsm::context_id ctx) override;

	//! \copydoc IFsm::CreateWalker
	virtual std::shared_ptr<fsm_walker_intf> create_walker()override;

public:
	//! \copydoc Fsm::IWalkable::GetNextStates
	virtual const std::vector<fsm::state_id>& get_next_states(const fsm::state_id& currentState, base::char_t ch) const override;
	
	//! \copydoc Fsm::IWalkable::GetContext
	virtual fsm::context_id get_context(const fsm::state_id& state) const override;

	//! \copydoc Fsm::IWalkable::GetStart
	virtual const fsm::state_id& get_start() const override;

protected:
	base::string dump() const;

private:
	void initialize();

	// KTTODO - move to fsm optimizier:
	void optimize(optimization_level_t level);
	void remove_epsilon_rules();
	bool remove_epsilon_rules_impl(fsm::state::holder& pState);
	void remove_unreachable_states();
	void determine();
	void minimize();

private:
	states_storage_t							m_states;			// All states in fsm.
	std::shared_ptr<fsm_context_factory_intf>	m_spContextFactory;	// Injected ctx factory.

	bool			m_optimized;	// After optimization fsm became readonly. KTTODO - Use local optimized copy to remove readonly property.
	fsm::state_id	m_start;		// Id of start state.

// All FSM manipulators:
	friend class fsm::builder;
};


}
