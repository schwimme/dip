#pragma once


namespace sys
{
namespace debug
{
namespace detail
{


void AssertImpl(const char *file, int line, const char *msg);
void BreakPointImpl();

}
}
}
