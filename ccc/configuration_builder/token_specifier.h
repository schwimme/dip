#pragma once

#include "token_specifier_intf.h"
#include <memory>


namespace ccc
{

class token_specifier:
	public token_specifier_intf
{
public:
	struct factory
	{
		virtual std::shared_ptr<token_specifier_intf> create_token_specifier(const sys::string& input_configuration) const
		{
			std::shared_ptr<token_specifier> sp_impl = std::make_shared<token_specifier>();
			sp_impl->initialize(input_configuration);
			return sp_impl;
		}
	};

public:
	virtual const sys::string serialize(std::vector<uint32_t>& ignored_tokens) const override;

private:
	void initialize(const sys::string& input_configuration);
	void build_cpp();
	void build_ps();

private:
	std::vector<std::vector<token_descriptor_e>>            m_priorities;
	std::vector<std::pair<token_descriptor_e, sys::string>> m_tokens;
};

}
