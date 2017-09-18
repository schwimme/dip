#pragma once


namespace Base
{
namespace Debugging
{
namespace detail
{


void AssertImpl(const char *file, int line, const char *msg);
void BreakPointImpl();

}
}
}
