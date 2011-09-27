/*_ _ismbbld.c						*/
/* Copyright (C) 1994-1995 by Digital Mars	*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>

/*
 *	Non-Macro version of _ismbblead.
 */

#ifdef _ismbblead
#undef _ismbblead
#endif

int __cdecl _ismbblead (unsigned int ch) {
  return (_mbctype + 1)[(unsigned char) ch] & _M1;
}

#else

#include <mbctype.h>
 /*
  * Non-Macro version of _ismbblead
  */
#ifdef _ismbblead
#undef _ismbblead
#endif
int _ismbblead(unsigned int c)
{
     return ((_mbctype+1)[(unsigned char)(c)] & _M1);
}

#endif

