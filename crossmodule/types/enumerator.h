#pragma once


namespace cross
{


template<typename T>
struct enumerator
{
	virtual const T* const get() const = 0;
	virtual void next() = 0;
	virtual void reset() = 0;
};


}
