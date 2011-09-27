/*_ mbsinc.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

unsigned char * _mbsinc (const unsigned char *sp)
{
    if (__mbcodepage != 0)
    {
      if (_ismbblead (*(sp++)) )
      {
          sp++;
      }
      return (unsigned char *) sp;
    }
    return (unsigned char *) sp + 1;
}

#endif

