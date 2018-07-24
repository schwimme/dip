#include "checker.h"
#include "configurable_fsm_factory.h"
#include <crossmodule/adapters/basestring.h>
#include <crossmodule/adapters/vector.h>
#include <checker_intf/checker_errors.h>

#include <error/exceptions.h>


namespace checklib
{

void checker_impl::configure(incident_handler_intf& pHandler, const sys::string& la_config, const sys::string& sa_config, const std::vector<uint32_t>& ignored_tokens)
{
	m_ignored_tokens = ignored_tokens;
	m_p_handler = &pHandler;
	prepare_base();
	configure_fsm(la_config);
	configure_pda(sa_config);
}


void checker_impl::check(const std::list<sys::string>& files)
{
	for (const sys::string& f : files)
	{
		worker_procedure(f); 
	}
}


void checker_impl::worker_procedure(const sys::string& file)
{
	// KTTODO crossmodule!
	std::shared_ptr<base::fsm_walker_intf> spFsmWalker = m_spFsm->create_walker();

	base::ll_validator_walker_intf* pPdaWalker;
	THROW_FAIL(m_spPda->create_walker(pPdaWalker, /*start_non_term*/1, /*indent_token*/0x20, /*begin_indent_depth*/0)); // KTTODO - toto je odrb - musi byt injecnute cez config (1 - nonterm program, 0x20 - tab)
	std::shared_ptr<base::ll_validator_walker_intf> spPdaWalker(pPdaWalker); // KTTODO - attacher

	std::shared_ptr<worker_intf> spWorker = create_worker(spFsmWalker, spPdaWalker, *m_p_handler, m_ignored_tokens);

	spWorker->check(file);
}


void checker_impl::configure_fsm(const sys::string& configuration)
{
	std::shared_ptr<la_cfg_builder_intf> spCfgBuilder = create_la_cfg_builder();
	std::shared_ptr<la_cfg> spCfg = spCfgBuilder->build(configuration);

	std::shared_ptr<configurable_fsm_ctx_factory> spCtxFactory 
		= std::make_shared<configurable_fsm_ctx_factory>(spCfg->m_priorityGroups);

	std::shared_ptr<base::fsm_intf> spFsm;
	THROW_FAIL(m_spBase->create_fsm(spFsm, spCtxFactory));

	base::fsm::state_id idleState = spFsm->generate_state(configurable_fsm_ctx_factory::INVALID_CTX);
	spFsm->set_start(idleState);

	for (const auto& it : spCfg->m_tokens)
	{
		spFsm->add_regex(idleState, cross::sys_string_on_string_ref(it.second), it.first, configurable_fsm_ctx_factory::INVALID_CTX);
	}

	m_spFsm = spFsm;
}


void checker_impl::configure_pda(const sys::string& configuration)
{
	std::shared_ptr<sa_cfg_builder_intf> spCfgBuilder = create_sa_cfg_builder();
	std::shared_ptr<sa_cfg> spCfg = spCfgBuilder->build(configuration);

	THROW_FAIL(m_spBase->create_ll_validator(m_spPda));

	for (const auto& it : spCfg->m_rules)
	{
		m_spPda->add_grammar_rule(it.parent, it.successors);
	}
}


void checker_impl::prepare_base()
{
	m_spBaseLoader = create_base_loader();

	base::base_intf* pBase = nullptr;
	m_spBaseLoader->get_base(pBase);

	m_spBase.reset(pBase);
}


}