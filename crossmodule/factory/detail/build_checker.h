#pragma once


#if !defined(BASE_LIB) && !defined(BASE_DLL) && !defined(BASE_NONE)
#	error "BASE linkage is not defined"
#endif


#if !defined(CHECKER_LIB) && !defined(CHECKER_DLL) && !defined(CHECKER_NONE)
#	error "CHECKER linkage is not defined"
#endif


#if !defined(CCC_LIB) && !defined(CCC_DLL) && !defined(CCC_NONE)
#	error "CCC linkage is not defined"
#endif
