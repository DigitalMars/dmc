/*_ fstrstr.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<stdio.h>
#include	<ctype.h>
#include	<stddef.h>
#include	<string.h>
#include	<stdlib.h>

#if __INTSIZE == 4 && (M_UNIX || M_XENIX || __OS2__ || __NT__)
#define __FAR
#else
#define __FAR __far
#endif

#include <limits.h>    /* defines UCHAR_MAX */
/****************************************************************************
See "Algorithms" Second Edition by Robert Sedgewick.  Boyer-Moore string
search routine.
****************************************************************************/
char __FAR * __FAR fstrstr(const char __FAR *text, const char __FAR * pattern)
{
  const size_t M = fstrlen(pattern);
  const unsigned long N = fstrlen(text);
  const char __FAR *p_p = pattern;
  const char __FAR *t_p;
  size_t skip[UCHAR_MAX + 1];
  size_t i, j;

  if(M == 0)
    return (char __FAR *)text;

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
    skip[*(const unsigned char __FAR *)p_p] = M - 1 - (p_p - pattern);
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
	return (char __FAR *)t_p;

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

