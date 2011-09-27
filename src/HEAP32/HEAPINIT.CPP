/* Copyright (C) 1995 by Digital Mars. $Revision: 1.1.1.1 $ */

#include "multpool.h"

extern "C"  void _allocinit()
{
    static int heap_inited = 0;
    
    if (!heap_inited)
    {
        RTLMultiPool::CreateMainHeap();
        heap_inited++;
    }
}


extern "C" void _allocwrap()
{
}

