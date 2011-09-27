//_ adelete.cpp
// Copyright (C) 1995 by Digital Mars
// All Rights Reserved

#if !_ENABLE_ARRAYNEW
#error "This module must be compiled with -Aa"
#endif

#include <new.h>

/************
 */
void operator delete[](void *block)
{
    delete block;
}
