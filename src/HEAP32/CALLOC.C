/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */

#include <stdlib.h>
#include <string.h>

void *calloc (size_t count, size_t each)
{
    void *block;
    size_t size;
  
    size = count * each;
    block = malloc (size);
    if (block == NULL)
        return NULL;
    memset (block, '\0', size);
    return block;
}

