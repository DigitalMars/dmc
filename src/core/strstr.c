/*_ strstr.c */
/* Copyright (C) 1985-2001 by Digital Mars		*/
/* All Rights Reserved					*/
/* www.digitalmars.com					*/
/* Written by Walter Bright				*/

#include	<stdio.h>
#include	<ctype.h>
#include	<stddef.h>
#include	<string.h>
#include	<stdlib.h>

#if 0 /* Smaller but slower under many circumstances. */
char *strstr(const char *s1,const char *s2)
{   size_t len2;
    size_t len1;
    char c2 = *s2;

    len1 = strlen(s1);
    len2 = strlen(s2);
    if (!len2)
	return (char *) s1;
    while (len2 <= len1)
    {
	if (c2 == *s1)
	    if (memcmp(s2,s1,len2) == 0)
		return (char *) s1;
	s1++;
	len1--;
    }
    return NULL;
}
#else
#include <limits.h>    /* defines UCHAR_MAX */
/****************************************************************************
See "Algorithms" Second Edition by Robert Sedgewick.  Boyer-Moore string
search routine.
****************************************************************************/
char *strstr(const char *text, const char * pattern)
{
  const size_t M = strlen(pattern);
  const unsigned long N = strlen(text);
  const char *p_p = pattern;
  const char *t_p;
  size_t skip[UCHAR_MAX + 1];
  size_t i, j;

  if(M == 0)
    return (char *)text;

  if(M > N)   /* If pattern is longer than the text string. */
    return 0;

#if __INTSIZE == 4
  _memintset((int *)skip, M, UCHAR_MAX + 1);
#else
  { size_t *s_p = skip + UCHAR_MAX;
    do
    {
      *s_p = M;
    }while(s_p-- > skip);
  }
#endif

  p_p = pattern;
  do
  {
    skip[*(const unsigned char *)p_p] = M - 1 - (p_p - pattern);
  } while(*++p_p);

  p_p = pattern + M - 1;
  t_p = text + M - 1;

  while(1)
  {
    char c;

    c = *t_p;
    if(c == *p_p)
    {
      if(p_p - pattern == 0)
	return (char *)t_p;

      t_p--; p_p--;
    }
    else
    {
      size_t step = M - (p_p - pattern);

      if (step < skip[(unsigned char)c])
	step = skip[(unsigned char)c];

      /* If we have run out of text to search in. */
      /* Need cast for case of large strings with 16 bit size_t... */
      if((unsigned long)(t_p - text) + step >= N)
	return 0;

      t_p += step;

      p_p = pattern + M - 1;
    }
  }
}
#endif	/* #if 0 */
