#pragma once

#include <cstdint>

using error_t = int32_t;


namespace detail
{
namespace mask
{
namespace severity
{
	static const error_t ERROR   = 0xC0000000;
	static const error_t WARNING = 0x80000000;
	static const error_t INFO    = 0x40000000;
}

namespace product
{
	static const error_t BASE    = 0x08000000;
	static const error_t CHECKER = 0x04000000;
	static const error_t CCC     = 0x02000000;
}
}


}

static const error_t E_NO_ERROR                                              = 0x00000000;
static const error_t E_COMMON_ERROR          = detail::mask::severity::ERROR | 0x00000001;
static const error_t E_MEMORY_ERROR          = detail::mask::severity::ERROR | 0x00000002;
static const error_t E_NOT_IMPLEMENTED_ERROR = detail::mask::severity::ERROR | 0x00000003;


namespace
{
	bool FAILED(error_t e) { return (e & detail::mask::severity::ERROR) == detail::mask::severity::ERROR; }
	bool SUCCEEDED(error_t e) { return !FAILED(e); }
}
