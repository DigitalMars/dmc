/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */

#include <stdlib.h>
#include "multpool.h"

#if __cplusplus
extern "C" {
#endif

size_t _msize (void *block)
{
    return  RTLMultiPool::GetMainHeap()->GetSize(block);  
}

#if __cplusplus
}
#endif





