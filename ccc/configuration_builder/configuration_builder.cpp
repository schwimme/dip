#include "configuration_builder.h"

namespace ccc
{

void configuration_builder::build_configuration(sys::string& la_cfg, sys::string& sa_cfg, std::vector<uint32_t>& ignored_tokens, const sys::string& input_config) const
{
	auto sp_token_specifier = create_token_specifier(input_config);
	la_cfg = sp_token_specifier->serialize(ignored_tokens);

	auto sp_cfa_specifier = create_cfa_specifier(input_config);
	sa_cfg = sp_cfa_specifier->serialize();
}


}
