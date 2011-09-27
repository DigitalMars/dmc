
/*_ wcstok.c
 * Copyright (C) 1995-2003 by Digital Mars, www.digitalmars.com
 * All Rights Reserved
 * Written by Walter Bright
 */

#ifdef _WIN32

#include <wchar.h>
#include <string.h>
#if _MT
#include <mt.h>
#endif

wchar_t * __CLIB wcstok(wchar_t * restrict s1, const wchar_t * restrict s2,
	wchar_t ** restrict ptr)
{
    if (!ptr)
    {
	// For compatibility with tchar._tcstok
#if _MT
	ptr = &_getthreaddata()->t_wcstok;
#else
	static wchar_t *_pnw = NULL;
	ptr = &_pnw;
#endif
    }

    if (s1 == NULL)
    {
	s1 = *ptr;
	if (s1 == NULL)
	    return NULL;
    }
    s1 += wcsspn(s1, s2);
    if (*s1 == L'\0')
	return NULL;
    *ptr = wcspbrk(s1, s2);
    if (*ptr != NULL)
    {
	**ptr = L'\0';
	(*ptr)++;
    }
    return s1;
}

#endif

/**/
