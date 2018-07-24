#include "impl.h"
#include <error/exceptions.h>

#include <crossmodule/adapters/basestring.h>
#include <crossmodule/adapters/vector.h>


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


void checking_module_impl::configure(const sys::string& config_path, checklib::incident_handler_intf& pHandler)
{
	configure_checklib(config_path);

	m_sp_cfg_builder = create_configuration_builder();
	sys::string la, sa;
	std::vector<uint32_t> ignored_tokens;
	m_sp_cfg_builder->build_configuration(la, sa, ignored_tokens, config_path);

	cross::std_vector_on_enumerator<uint32_t> adapter(ignored_tokens);
	THROW_FAIL(m_sp_checker->configure(&pHandler, cross::sys_string_on_string_ref(la), cross::sys_string_on_string_ref(sa), &adapter));

	// Run checking thread:
	m_check_files_thread = std::thread([&] { check_files_impl(); });

	return;
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

			to_process = std::move(m_files_to_check);
			m_files_to_check = {};
		}

		std::vector<cross::string_ref> cmt_to_process;
		for (const auto& it : to_process)
		{
			cmt_to_process.push_back(cross::sys_string_on_string_ref(it));
		}

		cross::std_vector_on_enumerator<cross::string_ref> adapter(cmt_to_process);
		THROW_FAIL(m_sp_checker->check(&adapter));
	}
}


void checking_module_impl::configure_checklib(const sys::string& path)
{
	m_sp_checklib_loader = create_checklib_loader();

	checklib::checklib_intf* pChecklib = nullptr;
	m_sp_checklib_loader->get_checklib(pChecklib);
	m_sp_checklib.reset(pChecklib);

	checklib::checker_intf* pChecker = nullptr;
	THROW_FAIL(m_sp_checklib->create_checker(pChecker));

	m_sp_checker.reset(pChecker);
}


}