//_ adelete2.cpp
// Copyright (C) 2006 by Digital Mars
// All Rights Reserved

#if !_ENABLE_ARRAYNEW
#error "This module must be compiled with -Aa"
#endif

#include <new.h>

/************
 */
void operator delete[](void *block, void *q) throw()
{
}
