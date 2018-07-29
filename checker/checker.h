#pragma once


#include <list>
#include <sys/types/string.h>
#include <base_loader/base_loader.h>
#include <base_intf/base_intf.h>
#include <sys/threadpool/threadpool.h>

#include "la_cfg_builder.h"
#include "sa_cfg_builder.h"
#include "checker_worker.h"

#include <mutex>
#include "checker_intf/checker_intf.h"
#include <sys/event_distributor/event_distributor.h>
#include "base_intf/ll_validator/ll_validator_intf.h"


namespace checklib
{


class checker_impl:
	protected base::base_loader::factory,
	protected la_cfg_builder::factory,
	protected sa_cfg_builder::factory,
	protected worker::factory
{
public:
	void configure(incident_handler_intf& pHandler, const sys::string& la_config, const sys::string& sa_config, const std::vector<uint32_t>& ignored_tokens);
	void check(const std::list<sys::string>& files);

private:
	void prepare_base();
	void configure_fsm(const sys::string& configuration);
	void configure_pda(const sys::string& configuration);
	void worker_procedure(const sys::string& file);

private:
	std::shared_ptr<base::base_loader_intf>      m_spBaseLoader;
	std::shared_ptr<base::base_intf>             m_spBase;
	std::shared_ptr<base::fsm_intf>              m_spFsm;
	std::shared_ptr<base::ll_validator_intf>     m_spPda;

	std::vector<uint32_t> m_ignored_tokens;

//	mutable sys::thread_pool<8> m_threadPool;

	incident_handler_intf* m_p_handler = nullptr;
};


}
