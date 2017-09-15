#pragma once


using ErrorCode = int32_t;

namespace detail
{
namespace Mask
{
namespace Severity
{
	static const ErrorCode ERROR	= 0x80000000;
	static const ErrorCode WARNING	= 0x40000000;
}

namespace Product
{
	static const ErrorCode BASE		= 0x08000000;
}
}

}

static const ErrorCode ERR_BASE_ALOCATION_FAILED = ERROR & BASE & 0x0001;

