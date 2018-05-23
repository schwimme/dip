#include "checker.h"
#include "configurable_fsm_factory.h"
#include <crossmodule/adapters/basestring.h>
#include <crossmodule/adapters/vector.h>
#include <checker_intf/checker_errors.h>

#include <error/exceptions.h>


namespace checklib
{

void checker_impl::configure(incident_handler_intf& pHandler, const sys::string& la_config, const sys::string& sa_config)
{
	m_p_handler = &pHandler;
	prepare_base();
	configure_fsm(la_config);
	configure_pda(sa_config);
}


void checker_impl::check(const std::list<sys::string>& files)
{
	for (const sys::string& f : files)
	{
//		m_threadPool.push([&, file = f]
//		{	
			worker_procedure(f); 
//		});
	}
}


void checker_impl::worker_procedure(const sys::string& file)
{
	// KTTODO crossmodule!
	std::shared_ptr<base::fsm_walker_intf> spFsmWalker = m_spFsm->create_walker();

	base::pda_walker_intf* pPdaWalker;
	m_spPda->create_walker(pPdaWalker);
	std::shared_ptr<base::pda_walker_intf> spPdaWalker(pPdaWalker); // KTTODO - attacher

	std::shared_ptr<worker_intf> spWorker = create_worker(spFsmWalker, spPdaWalker, *m_p_handler);

	spWorker->check(file);
}


void checker_impl::configure_fsm(const sys::string& configuration)
{
	std::shared_ptr<la_cfg_builder_intf> spCfgBuilder = create_la_cfg_builder();
	std::shared_ptr<la_cfg> spCfg = spCfgBuilder->build(configuration);

	std::shared_ptr<configurable_fsm_ctx_factory> spCtxFactory 
		= std::make_shared<configurable_fsm_ctx_factory>(spCfg->m_priorityGroups);

	std::shared_ptr<base::fsm_intf> spFsm;
	error_t errorCode = m_spBase->create_fsm(spFsm, spCtxFactory);
	if (FAILED(errorCode))
	{
		throw exception_t(TEXT("Failed to get fsm from base"), errorCode);
	}

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

	base::pda_intf* pPda = nullptr;
	error_t errorCode = m_spBase->create_pda(pPda);
	if (FAILED(errorCode))
	{
		throw exception_t(TEXT("Failed to get pda from base"), errorCode);
	}
	std::shared_ptr<base::pda_intf> spPda(pPda); // KTTODO - sp attacher

	for (const auto& it : spCfg->m_rules)
	{
		spPda->add_rule(
			it.m_token,
			cross::std_vector_on_enumerator<sa_rule::stack_item>(it.m_stackTop),
			cross::std_vector_on_enumerator<sa_rule::stack_item>(it.m_stackReplace));
	}

	m_spPda = spPda;
}


void checker_impl::prepare_base()
{
	m_spBaseLoader = create_base_loader();

	base::base_intf* pBase = nullptr;
	m_spBaseLoader->get_base(pBase);

	m_spBase.reset(pBase);
}


}