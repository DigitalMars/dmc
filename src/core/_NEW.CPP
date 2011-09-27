/*_ _new.c   Tue May 30 1989   Modified by: Walter Bright */
/* Copyright (C) 1988-1989 by Northwest Software	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/

#include <stdlib.h>
#include <new.h>

void *__do_newalloc(size_t);


void * operator new(size_t m_size)
{   
   return __do_newalloc(m_size);
}
