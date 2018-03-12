#pragma once


#include <list>
#include <types/string.h>
#include <crossmodule/factory/objectfactory.h>
#include <base/base.h>
#include <base_intf/ThreadPool/threadpool.h>

#include "la_cfg_builder.h"
#include "sa_cfg_builder.h"
#include "checker_worker.h"

#include <mutex>


namespace checker
{


class checker_impl: // KTTODO - checker_intf
	public accident_handler,
	protected la_cfg_builder::factory,
	protected sa_cfg_builder::factory,
	protected worker::factory
{
public:
	void configure(const base::string& la_cfg_path, const base::string& sa_cfg_path);
	void check(const std::list<base::string>& files);

public:
	virtual void on_accident(const accident_info& info) override;

private:
	void prepare_base();
	void configure_fsm(const base::string& la_cfg_path);
	void configure_pda(const base::string& sa_cfg_path);
	void worker_procedure(const base::string& file);

private:
	std::shared_ptr<crossmodule::object_factory> m_spFactory;
	std::shared_ptr<base::base_intf>             m_spBase;
	std::shared_ptr<base::fsm_intf>              m_spFsm;
	std::shared_ptr<base::pda_intf>              m_spPda;

	mutable base::thread_pool<8> m_threadPool;

	std::mutex m_print_mutex;
};


}
