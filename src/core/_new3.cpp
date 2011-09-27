/*_ _new3.cpp */
/* Copyright (C) 2006 by Digital Mars			*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/

#include <stdlib.h>
#include <new.h>

void *__do_newalloc(size_t);


void * operator new(size_t m_size, const std::nothrow_t& t) throw()
{   
   return __do_newalloc(m_size);
}
