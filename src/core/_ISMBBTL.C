/*_ _ismbbtl.c						*/
/* Copyright (C) 1994-1995 by Digital Mars	*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>

/*
 *	Non-Macro version of _ismbbtrail.
 */

#ifdef _ismbbtrail
#undef _ismbbtrail
#endif

int __cdecl _ismbbtrail (unsigned int ch) {
  return (_mbctype + 1)[(unsigned char) ch] & _M2;
}

#else

#include <mbctype.h>

 /*
  * Non-Macro version of _ismbbtrail
  */
#ifdef _ismbbtrail
#undef _ismbbtrail
#endif
int _ismbbtrail(unsigned int c)
{
     return ((_mbctype+1)[(unsigned char)(c)] & _M2);
}

#endif

