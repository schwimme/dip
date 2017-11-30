#pragma once


namespace crossmodule
{

template<typename T>
struct enumerator
{
	virtual const T* const get() = 0;
	virtual void next() = 0;
	virtual void reset() = 0;
};


}
