//_ anew3.cpp
// Copyright (C) 1995-2006 by Digital Mars
// All Rights Reserved

#if !_ENABLE_ARRAYNEW
#error "This module must be compiled with -Aa"
#endif

#include <stdlib.h>
#include <new.h>

void *__do_newalloc(size_t);

/************
 */
void *operator new[](unsigned sz, const std::nothrow_t& t) throw()
{
    return __do_newalloc(sz);
}

