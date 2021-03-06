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
	static const error_t BASE     = 0x08000000;
	static const error_t CHECKLIB = 0x04000000;
	static const error_t CCC      = 0x02000000;
}
}


}

static const error_t E_NO_ERROR                                              = 0x00000000;
static const error_t E_COMMON_ERROR          = detail::mask::severity::ERROR | 0x00000001;
static const error_t E_MEMORY_ERROR          = detail::mask::severity::ERROR | 0x00000002;
static const error_t E_NOT_IMPLEMENTED_ERROR = detail::mask::severity::ERROR | 0x00000003;
static const error_t E_INTF_NOT_FOUND_ERROR  = detail::mask::severity::ERROR | 0x00000004; // KTTODO - cross error?
static const error_t E_INVALID_PARAMETER     = detail::mask::severity::ERROR | 0x00000005;
static const error_t E_COULD_NOT_OPEN_FILE   = detail::mask::severity::ERROR | 0x00000006;

namespace
{
	bool FAILED(error_t e) { return (e & detail::mask::severity::ERROR) == detail::mask::severity::ERROR; }
	bool SUCCEEDED(error_t e) { return !FAILED(e); }
}

#ifndef RETURN_FAIL
#define RETURN_FAIL(e) do { error_t r = (e); if (FAILED(r)) { return r; } } while (0)
#endif

#ifndef THROW_FAIL
#define THROW_FAIL(e) do { error_t r = (e); if (FAILED(r)) { throw exception_t(r); } } while (0)
#endif
