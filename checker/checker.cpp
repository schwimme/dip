#include "checker.h"
#include <iostream> // KTTODO - probably remove
#include "configurable_fsm_factory.h"
#include "crossmodule\adapters\basestring.h"
#include "crossmodule\adapters\vector.h"

namespace checker
{

void checker_impl::configure(const base::string& la_cfg_path, const base::string& sa_cfg_path)
{
	prepare_base();
	configure_fsm(la_cfg_path);
	configure_pda(sa_cfg_path);
}


void checker_impl::check(const std::list<base::string>& files)
{
	for (const base::string& f : files)
	{
		// KTTODO do not use in UT.
//		m_threadPool.push([&, file = f]
//		{	
			worker_procedure(f); 
//		});
	}
}


void checker_impl::worker_procedure(const base::string& file)
{
	// KTTODO crossmodule!
	std::shared_ptr<base::fsm_walker_intf> spFsmWalker = m_spFsm->create_walker();

	base::pda_walker_intf* pPdaWalker;
	m_spPda->create_walker(pPdaWalker);
	std::shared_ptr<base::pda_walker_intf> spPdaWalker(pPdaWalker); // KTTODO - attacher

	std::shared_ptr<worker_intf> spWorker = create_worker(spFsmWalker, spPdaWalker, *this);

	spWorker->check(file);
}


void checker_impl::configure_fsm(const base::string& la_cfg_path)
{
	std::shared_ptr<la_cfg_builder_intf> spCfgBuilder = create_la_cfg_builder();
	std::shared_ptr<la_cfg> spCfg = spCfgBuilder->build(la_cfg_path);

	std::shared_ptr<configurable_fsm_ctx_factory> spCtxFactory 
		= std::make_shared<configurable_fsm_ctx_factory>(spCfg->m_priorityGroups);

	std::shared_ptr<base::fsm_intf> spFsm;
	if (m_spBase->create_fsm(spFsm, spCtxFactory) != 0) // KTTODO errors
	{
		throw "KTTODO - error";
	}

	base::fsm::state_id idleState = spFsm->generate_state(configurable_fsm_ctx_factory::INVALID_CTX);
	spFsm->set_start(idleState);

	for (const auto& it : spCfg->m_tokens)
	{
		spFsm->add_regex(idleState, crossmodule::base_string_on_string_ref(it.second), it.first, configurable_fsm_ctx_factory::INVALID_CTX);
	}

	m_spFsm = spFsm;
}


void checker_impl::configure_pda(const base::string& sa_cfg_path)
{
	std::shared_ptr<sa_cfg_builder_intf> spCfgBuilder = create_sa_cfg_builder();
	std::shared_ptr<sa_cfg> spCfg = spCfgBuilder->build(sa_cfg_path);

	base::pda_intf* pPda = nullptr;
	if (m_spBase->create_pda(pPda) != 0) // KTTODO errors
	{
		throw "KTTODO - error";
	}
	std::shared_ptr<base::pda_intf> spPda(pPda); // KTTODO - sp attacher

	for (const auto& it : spCfg->m_rules)
	{
		spPda->add_rule(
			it.m_token,
			crossmodule::std_vector_on_enumerator<sa_rule::stack_item>(it.m_stackTop),
			crossmodule::std_vector_on_enumerator<sa_rule::stack_item>(it.m_stackReplace));
	}

	m_spPda = spPda;
}


void checker_impl::prepare_base()
{
	// KTTODO injection:
	m_spFactory = std::make_shared<crossmodule::object_factory>();

	void* pBase;
	if (m_spFactory->get_object(GUID_BASE_V1, &pBase) != 0) // KTTODO error
	{
		throw ""; // KTTODO exception
	}
	m_spBase.reset((base::base_intf*)pBase);
}


void checker_impl::on_accident(const accident_info& info)
{
	std::lock_guard<std::mutex> l(m_print_mutex);
	std::cout << "[ACCIDENT] " << info.file << " " << info.line << ":" << info.col << " - " << info.errDesc << "\n";
}


}