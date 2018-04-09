#include "debug_impl.h"
#include <Windows.h>


namespace sys
{
namespace debug
{
namespace detail
{

	
void AssertImpl(const char *file, int line, const char *msg)
{
	(void)(file, line, msg);
	::DebugBreak();
}


void BreakPointImpl()
{
	while (::IsDebuggerPresent())
	{
		::Sleep(500);
	}

	::DebugBreak();
}



}
}
}
