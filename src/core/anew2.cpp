//_ anew2.cpp
// Copyright (C) 1995 by Digital Mars
// All Rights Reserved


#if !_ENABLE_ARRAYNEW
#error "This module must be compiled with -Aa"
#endif

#include <new.h>

/************
 * Overload of the operator new[] to take 2 arguments.
 */
void *operator new[](unsigned sz, void *p) throw()
{
	return (p);
}


