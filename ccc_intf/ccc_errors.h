#pragma once


#include <error/error_code_base.h>
#include <sys/types/string.h>


#define MAKE_CCC_ERROR_CODE(name, code) \
static const error_t E_CCC_##name##_ERROR = detail::mask::severity::ERROR |detail::mask::product::CCC | code

#define MAKE_CCC_WARNING_CODE(name, code) \
static const error_t E_CCC_##name##_WARN = detail::mask::severity::WARNING |detail::mask::product::CCC | code

#define MAKE_CCC_INFO_CODE(name, code) \
static const error_t E_CCC_##name##_INFO = detail::mask::severity::INFO |detail::mask::product::CCC | code


MAKE_CCC_WARNING_CODE(UNSPECIFIED_HANDLER, 0x00000001);


#undef MAKE_CCC_INFO_CODE
#undef MAKE_CCC_WARNING_CODE
#undef MAKE_CCC_ERROR_CODE