#pragma once


namespace ccc
{
	
struct accident_handler_intf
{

	struct info
	{
		// KTTODO
	};
	// KTTODO
	virtual void on_ccc_accident(const info* p_info) noexcept { (void)(p_info); }

protected:
	virtual ~accident_handler_intf() {}
};


}
