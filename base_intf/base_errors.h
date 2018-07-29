#pragma once


#include <error/error_code_base.h>


#define MAKE_BASE_ERROR_CODE(name, code) \
static const error_t E_BASE_##name##_ERROR = detail::mask::severity::ERROR |detail::mask::product::BASE | code

#define MAKE_BASE_WARINING_CODE(name, code) \
static const error_t E_BASE_##name##_WARN = detail::mask::severity::WARNING |detail::mask::product::BASE | code

#define MAKE_BASE_INFO_CODE(name, code) \
static const error_t E_BASE_##name##_INFO = detail::mask::severity::INFO |detail::mask::product::BASE | code


MAKE_BASE_ERROR_CODE(UNEXPECTED_TOKEN, 0x00000001);
MAKE_BASE_ERROR_CODE(UNEXPECTED_END_OF_FILE, 0x00000002);


#undef MAKE_BASE_INFO_CODE
#undef MAKE_BASE_WARINING_CODE
#undef MAKE_BASE_ERROR_CODE