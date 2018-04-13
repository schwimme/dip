#pragma once


#include <sys/types/string.h>
#include "cfa_specifier_intf.h"
#include <memory>


#include "token_specifier_intf.h"


namespace ccc
{

class cfa_specifier:
	public cfa_specifier_intf
{
public:
	struct factory
	{
		virtual std::shared_ptr<cfa_specifier_intf> create_cfa_specifier(const sys::string& configuration) const
		{
			auto sp_impl = std::make_shared<cfa_specifier>();
			sp_impl->initialize(configuration);
			return sp_impl;
		}
	};

private:
	enum class stack_item_e:
		uint32_t
	{
		si_class,
		si_space,
		si_id,
		si_cascaded_id,
		si_enter,
		si_class_impl,
		si_inheritance,
		si_inheritance_impl,
		si_inheritance_optional,
		si_visibility,
		si_tab
	};

	struct rule
	{
		token_descriptor_e m_token;
		std::vector<stack_item_e> m_stack_top;
		std::vector<stack_item_e> m_stack_replace;
	};

	using rules = std::vector<rule>;

public:
	virtual sys::string serialize() const override;

private:
	void initialize(const sys::string& configuration);

	void build_class();
	void build_inheritance();

private:
	rules m_rules;
};

}
