#include "impl.h"
#include <error/exceptions.h>

#include <crossmodule/adapters/basestring.h>

namespace ccc
{

checking_module_impl::~checking_module_impl()
{
	{
		std::lock_guard<std::mutex> l(m_files_to_check_mutex);
		m_stopping = true;
	}
	m_files_to_check_cv.notify_one();

	if (m_check_files_thread.joinable())
	{
		m_check_files_thread.join();
	}
}


std::shared_ptr<sys::registrator_intf> checking_module_impl::configure(const sys::string& config_path, accident_handler_intf& pHandler)
{
	configure_checklib(config_path);

	m_sp_cfg_builder = create_configuration_builder();
	sys::string la, sa;
	m_sp_cfg_builder->build_configuration(la, sa, config_path);

	error_t errorCode = m_sp_checker->configure(cross::sys_string_on_string_ref(la), cross::sys_string_on_string_ref(sa));
	if (FAILED(errorCode))
	{
		throw exception_t(errorCode);
	}

	auto sp = m_event_distributor.register_function([&pHandler](accident_handler_intf::info* p_info)
	{
		pHandler.on_ccc_accident(p_info);
	});

	// Run checking thread:
	m_check_files_thread = std::thread([&] { check_files_impl(); });

	return sp;
}


void checking_module_impl::check_files(const std::vector<sys::string>& files)
{
	{
		std::lock_guard<std::mutex> l(m_files_to_check_mutex);
		m_files_to_check.insert(m_files_to_check.end(), files.begin(), files.end());
	}

	m_files_to_check_cv.notify_one();
}


void checking_module_impl::check_files_impl()
{
	while (1)
	{
		std::vector<sys::string> to_process;
		{
			std::unique_lock<std::mutex> l(m_files_to_check_mutex);
			m_files_to_check_cv.wait(l, [&] { return (m_files_to_check.empty() == false) || m_stopping; });

			if (m_stopping)
			{
				return;
			}

			to_process = m_files_to_check;
			m_files_to_check = {};
		}
	}
}


void checking_module_impl::configure_checklib(const sys::string& path)
{
	m_sp_checklib_loader = create_checklib_loader();

	checklib::checklib_intf* pChecklib = nullptr;
	m_sp_checklib_loader->get_checklib(pChecklib);
	m_sp_checklib.reset(pChecklib);

	checklib::checker_intf* pChecker = nullptr;
	if (FAILED(m_sp_checklib->create_checker(pChecker)))
	{
		throw exception_t(E_INTF_NOT_FOUND_ERROR);
	}
	m_sp_checker.reset(pChecker);
}


}