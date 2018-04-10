#pragma once


#include <error/error_code_base.h>
#include <sys/types/string.h>


#define MAKE_CHECKER_ERROR_CODE(name, code) \
static const error_t E_CHECKER_##name##_ERROR = detail::mask::severity::ERROR |detail::mask::product::CHECKER | code

#define MAKE_CHECKER_WARINING_CODE(name, code) \
static const error_t E_CHECKER_##name##_WARN = detail::mask::severity::WARNING |detail::mask::product::CHECKER | code

#define MAKE_CHECKER_INFO_CODE(name, code) \
static const error_t E_CHECKER_##name##_INFO = detail::mask::severity::INFO |detail::mask::product::CHECKER | code


MAKE_CHECKER_ERROR_CODE(SOME_TEST, 0x00000000);


#undef MAKE_CHECKER_INFO_CODE
#undef MAKE_CHECKER_WARINING_CODE
#undef MAKE_CHECKER_ERROR_CODE