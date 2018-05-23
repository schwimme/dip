#pragma once

#include <sys/types/string.h>
#include <vector>

#include <sys/event_distributor/event_distributor.h>
#include <ccc_intf/ccc_accident_handler.h>

#include <thread>
#include <mutex>

#include <condition_variable>

#include <checklib_loader/checklib_loader.h>
#include <checker_intf/checklib_intf.h>
#include <checker_intf/checker_intf.h>

#include <ccc/configuration_builder/configuration_builder.h>


namespace ccc
{

class checking_module_impl:
	protected checklib::checklib_loader::factory,
	protected configuration_builder::factory
{
public:
	void configure(const sys::string& config_path, checklib::incident_handler_intf& pHandler);
	void check_files(const std::vector<sys::string>& files);

public:
	~checking_module_impl();

protected:
	void configure_checklib(const sys::string& path);


private: // checklib
	std::shared_ptr<checklib::checklib_loader_intf> m_sp_checklib_loader;
	std::shared_ptr<checklib::checklib_intf>        m_sp_checklib;
	std::shared_ptr<checklib::checker_intf>         m_sp_checker;

private:
	void check_files_impl();
	std::thread m_check_files_thread;

private:
	std::shared_ptr<configuration_builder_intf> m_sp_cfg_builder;
		
	bool                     m_stopping = false;
	std::mutex               m_files_to_check_mutex;
	std::condition_variable  m_files_to_check_cv;
	std::vector<sys::string> m_files_to_check;
	sys::event_distributor<accident_handler_intf::info*> m_event_distributor;
};

}
