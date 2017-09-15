#pragma once


#include "defs.h"


namespace Base
{
	
	
struct IFsmWalker
{
	virtual bool ProcessStep(Fsm::AlphabetType ch) = 0;
	virtual bool VerifyLiteral(const String& literal) = 0;
	virtual void Reset() = 0;
	virtual bool IsInFiniteState() = 0;
};
	

}