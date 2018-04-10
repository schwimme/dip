#pragma once


#include <list>
#include <sys/types/string.h>
#include <crossmodule/factory/objectfactory.h>
#include <base/base.h>
#include <sys/threadpool/threadpool.h>

#include "la_cfg_builder.h"
#include "sa_cfg_builder.h"
#include "checker_worker.h"

#include <mutex>


namespace checker
{


class checker_impl:
	public accident_handler,
	protected cross::object_factory::factory,
	protected la_cfg_builder::factory,
	protected sa_cfg_builder::factory,
	protected worker::factory
{
public:
	void configure(const sys::string& la_cfg_path, const sys::string& sa_cfg_path);
	void check(const std::list<sys::string>& files);

public:
	// KTTODO - automaticka naprava
	virtual void on_accident(const accident_info& info) override;

private:
	void prepare_base();
	void configure_fsm(const sys::string& la_cfg_path);
	void configure_pda(const sys::string& sa_cfg_path);
	void worker_procedure(const sys::string& file);

private:
	std::shared_ptr<cross::object_factory_intf>  m_spFactory;
	std::shared_ptr<base::base_intf>             m_spBase;
	std::shared_ptr<base::fsm_intf>              m_spFsm;
	std::shared_ptr<base::pda_intf>              m_spPda;

	mutable sys::thread_pool<8> m_threadPool;

	std::mutex m_print_mutex;
};


}
