#pragma once


#include <error/error_code_base.h>
#include <sys/types/string.h>


#define MAKE_CHECKLIB_ERROR_CODE(name, code) \
static const error_t E_CHECKLIB_##name##_ERROR = detail::mask::severity::ERROR |detail::mask::product::CHECKLIB | code

#define MAKE_CHECKLIB_WARINING_CODE(name, code) \
static const error_t E_CHECKLIB_##name##_WARN = detail::mask::severity::WARNING |detail::mask::product::CHECKLIB | code

#define MAKE_CHECKLIB_INFO_CODE(name, code) \
static const error_t E_CHECKLIB_##name##_INFO = detail::mask::severity::INFO |detail::mask::product::CHECKLIB | code


MAKE_CHECKLIB_ERROR_CODE(PARSING, 0x00000001);


#undef MAKE_CHECKLIB_INFO_CODE
#undef MAKE_CHECKLIB_WARINING_CODE
#undef MAKE_CHECKLIB_ERROR_CODE