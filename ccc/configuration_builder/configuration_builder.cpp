#include "configuration_builder.h"

namespace ccc
{

void configuration_builder::build_configuration(sys::string& la_cfg, sys::string& sa_cfg, const sys::string& input_config) const
{
	auto sp_token_specifier = create_token_specifier(input_config);
	la_cfg = sp_token_specifier->serialize();

	auto sp_cfa_specifier = create_cfa_specifier(input_config);
	sa_cfg = sp_cfa_specifier->serialize();
}


}
