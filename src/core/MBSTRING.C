/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 */

#ifndef _MBCS

#include <mbstring.h>
#include <ctype.h>
#include <mbctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if _MT
#include	"mt.h"
#endif

/*
 * Increment the Multi-Byte Character string S by one char
 */
unsigned char *_mbsinc(const unsigned char *s)
{
   return ((_ismbblead(*(s)++) && (*(s))) ? ++((unsigned char *)s) : ((unsigned char *)s)); 
}

/*************************
 * Compare strings, ignoring differences in case
 *	jstricmp.
 */
int _mbsicmp(const unsigned char *p1, const unsigned char *p2)
{
	while (*p1 && *p2)
	{	if (_mbsnextc(p1) != _mbsnextc(p2))
		{	if (isalpha(*p1) && isalpha(*p2))
			{	int i;

				i = toupper(*p1) - toupper(*p2);
				if (i)
					return i;
			}
			else if (_ismbcalpha(_mbsnextc(p1)) && _ismbcalpha(_mbsnextc(p2)))
					{	int i;

						i = _mbctoupper(_mbsnextc(p1)) - _mbctoupper(_mbsnextc(p2));
						if (i)
							return i;
					}
			else 
				break;
		}
		p1 = _mbsinc((unsigned char *)p1);
		p2 = _mbsinc((unsigned char *)p2);
	}
	return _mbsnextc(p1) - _mbsnextc(p2);
}

/**********************
 * Reverse the characters in the string p.
 * Returns:
 *	p
 */
unsigned char *_mbsrev(unsigned char *p)
{	wchar_t *pend,*pstart, w_t;
	short len = _mbslen(p);
	wchar_t *pnew = malloc((len+1)*sizeof(wchar_t));
	
	pstart = pnew;

	mbstowcs(pnew, (const char *)p, len);
	pend = pnew + len- 1;
	while (pnew < pend)
	{	w_t = *pnew;
		*pnew++ = *pend;	/* swap *p and *pend		*/
		*pend-- = w_t;
	}
	wcstombs((char *)p, pstart, len);
	free(pnew);
	return p;
}

/************************************************************************/
/*  Compares N bytes of two strings, case.sensitive                     */
/*                                                                      */
/*  Copyright 1989-90 by Robert B. Stout dba MicroFirm                  */
/*  All rights reserved.                                                */
/*                                                                      */
/*  Parameters: 1 - first string                                        */
/*              2 - second string                                       */
/*              3 - max number of characters to compare                 */
/*                                                                      */
/*  Returns: zero     - if strings match                                */
/*           positive - if first > second                               */
/*           negative - if first < second                               */
/*                                                                      */
/*  Notes: If the specified number of characters to compare exceeds     */
/*         the length of either string, the search is terminated and    */
/*         the return value is the same as if strcmpl() were called.    */
/*                                                                      */
/************************************************************************/
int _mbsncmp(const unsigned char *s1, const unsigned char *s2, size_t n)
{
    unsigned short c1, c2;

    c1 = c2 = 0;
    while (n--) 
    {
	c1 = _mbsnextc(s1);
	c2 = _mbsnextc(s2);
	s1 = _mbsinc((unsigned char *)s1);
	s2 = _mbsinc((unsigned char *)s2);
	if (!c1 || (c1 = c1) != (c2 = c2))
    		break;
    }
    return c1 - c2;
}

int _mbsnbcmp(const unsigned char *s1, const unsigned char *s2, size_t n)
{
    unsigned short c1, c2;

    c1 = c2 = 0;
    while (n--) 
    {
	c1 = _mbsnextc(s1);
	c2 = _mbsnextc(s2);
	if (_ismbblead(*s1))
		n--;
	s1 = _mbsinc((unsigned char *)s1);
	s2 = _mbsinc((unsigned char *)s2);
	if (!c1 || (c1 = c1) != (c2 = c2))
    		break;
    }
    return c1 - c2;
}

/**************************
 *copy string q to string p, stop at n characters.
 * Returns:
 *	pointer to string p.
 */
unsigned char *_mbsnbcpy(unsigned char *p, const unsigned char *q, size_t n)
{
	unsigned char *first = p;

	if (!n)
		return first;

	while (n--)
	{	
      if (_ismbblead(*q)) {
		 if (_ismbbtrail(*(q+1)))
		   *p++ = *q++;
		 else {
		   *p++ = '\0';
		   q++;
		 }
		}

      *p++ = *q;

      if (*q)
        q++;
	}

	return first;
}

unsigned int _mbctoupper(unsigned int s)
{
//	return ( (_ismbclower(s)?(s-0x8281+0x8260):(((s)>>8)?(s):toupper(s))) );
   return toupper(s);
}

unsigned char *  _mbsdec(const unsigned char *p1, const unsigned char *p2)
{
	unsigned char *prev, *next = (unsigned char *)p1;

	if (p1 == p2)
		return NULL;
	if (NULL == p2)
		return NULL;
	do {
		prev = (unsigned char *)next;
		next = _mbsinc((unsigned char *)next);
	   } while (*next && (next < p2));

	return (unsigned char *)prev;
}

int _mbsnbicmp(const unsigned char *s1, const unsigned char *s2, size_t n)
{
    unsigned short c1, c2;

    c1 = c2 = 0;
    while (n--) 
    {
	if (_ismbblead(*s1) &&  _ismbblead(*s2)) {
		n--;
		c1 = _mbctoupper(_mbsnextc(s1));
		c2 = _mbctoupper(_mbsnextc(s2));
		s1 = _mbsinc((unsigned char *)s1);
		s2 = _mbsinc((unsigned char *)s2);
		if (!c1 || (c1 = _mbctoupper(c1)) != (c2 = _mbctoupper(c2)))
	    		break;
	} else {
		c1 = toupper(*s1);
		c2 = toupper(*s2);
		s1 = _mbsinc((unsigned char *)s1);
		s2 = _mbsinc((unsigned char *)s2);
		if (!c1 || (c1 = toupper(c1)) != (c2 = toupper(c2)))
	    		break;
	}
    }
    return c1 - c2;
}

size_t _mbclen(const unsigned char *cp)
{
	int i = 0;
	
	while (*cp) {
	  cp = _mbsinc((unsigned char *)cp);
	  i++;
	}
	
	return i;
}

/* character count */
size_t  _mbslen(const unsigned char *s)
{
	int i = 0;
	
	while (*s) {
	  s = _mbsinc((unsigned char *)s);
	  i++;
	}
	
	return i;
}

/*************************
 * Compare strings, case sensitive.
 *	_mbstrcmp
 */
int _mbscmp(const unsigned char *p1, const unsigned char *p2)
{
	while (*p1 && *p2)
	{	if (*p1 != *p2)
		{	if (isalpha(*p1) && isalpha(*p2))
			{	int i;

				i = *p1 - *p2;
				if (i)
					return i;
			}
			else if (_ismbcalpha(_mbsnextc(p1)) && _ismbcalpha(_mbsnextc(p2))) 
					{	int i;

						i = _mbsnextc(p1) - _mbsnextc(p2);
						if (i)
							return i;
					}
			else 
				break;
		}
		p1 = _mbsinc((unsigned char *)p1);
		p2 = _mbsinc((unsigned char *)p2);
	}
	return *p1 - *p2;
}

/***********************
 * Findfirst occurrence of char c in string p.
 * Returns:
 *	pointer to last occurrence
 *	NULL if not found
 */
unsigned char * _mbschr(const unsigned char *p, unsigned int c)
{
	if (p) do {
	  if (c == _mbsnextc(p))
	    return (unsigned char *)p;
	  p = _mbsinc((unsigned char *)p);
	} while(*p);
	
	return NULL;
}

/*
 * Return next character from Multi-Byte Character String
 */
unsigned int _mbsnextc(const unsigned char *p)
{
	if (_ismbblead(*p))
	  return ((*p)<<8) + *(++p);
	else
	  return *p;
}

/**********************************
 * Return pointer to first occurrence of string s2 in s1.
 * Return NULL if s1 is not in s2.
 */
unsigned char *  _mbsstr(const unsigned char *s1, const unsigned char *s2)
{   size_t len2;
    size_t len1;

    len1 = _mbslen(s1);
    len2 = _mbslen(s2);
    if (!len2)
	return (unsigned char *) s1;
    while (len2 <= len1)
    {
	if (_mbsnextc(s2) == _mbsnextc(s1))
	    if (memcmp(s2,s1,len2) == 0)
		return (unsigned char *) s1;
	if (_ismbblead(*s1))
	  len1--;
	len1--;
	s1 = _mbsinc((unsigned char *)s1);
    }
    return NULL;
}

/***********************
 * Return pointer to first occurrence in string p1 of any of the characters
 * in the string p2. Return NULL if no occurrences are found.
 */
unsigned char * _mbspbrk(const unsigned char *p1, const unsigned char *p2)
{	unsigned char *p2start = (unsigned char *)p2;

	while (*p1)
	{	while (*p2)
		{      if (_mbsnextc(p1) == _mbsnextc(p2))
				return (void *) p1;
			p2 = _mbsinc((unsigned char *)p2);
		}
		p2 = p2start;
		p1 = _mbsinc((unsigned char *)p1);
	}
	return NULL;
}

size_t _mbscspn(const unsigned char *p1, const unsigned char *p2)
{
	unsigned char *p1start = (unsigned char *)p1;
	unsigned char *p2start = (unsigned char *)p2;

	while (*p1)
	{	while (*p2)
		{      if (_mbsnextc(p1) == _mbsnextc(p2))
				goto L1;
			p2 = _mbsinc((unsigned char *)p2);
		}
		p2 = p2start;
		p1 = _mbsinc((unsigned char *)p1);
	}
    L1:
	return p1 - p1start;
}

/***********************
 * Find last occurrence of char c in string p.
 * Returns:
 *	pointer to last occurrence
 *	NULL if not found
 */

unsigned char * _mbsrchr(const unsigned char *p, unsigned int c)
{
	unsigned char *cp = NULL;
	if (p) do {
		if (_mbsnextc(p) == c)
			cp = (unsigned char *)p;
		p = _mbsinc((unsigned char *)p);
	} while (*p);
	
	return (unsigned char *)cp;
}

size_t  _mbsspn(const unsigned char *p1, const unsigned char *p2)
{	unsigned char *p1start = (unsigned char *)p1;
	unsigned char *p2start = (unsigned char *)p2;

	while (*p1)
	{	while (*p2)
		{      if (_mbsnextc(p1) == _mbsnextc(p2))
				goto L1;
			p2 = _mbsinc((unsigned char *)p2);
		}
		break;			/* it's not in p2		*/
	    L1:
		p2 = p2start;
		p1 = _mbsinc((unsigned char *)p1);
	}
	return p1 - p1start;
}

unsigned char * __cdecl _mbstok(unsigned char *p1, const unsigned char *p2)
{
#if _MT
	unsigned char **pps = (unsigned char **)&_getthreaddata()->t_strtok;
#define ps (*pps)
#else
	static unsigned char *ps = NULL;
#endif
	unsigned char *p2start = (unsigned char *)p2;

	if (!p1)
	{	p1 = ps;			/* use old value		*/
		if (!p1)
			return (unsigned char *) NULL;
	}
	p1 += _mbsspn(p1,p2);		/* find first char of token	*/
	if (!*p1)
		return (unsigned char *) NULL;
	ps = _mbspbrk((const unsigned char *)p1, p2);		/* find end of token		*/
	if (ps) {
	        if (_ismbblead(*ps))
	        	*ps++ = 0;
		*ps++ = 0;
	}							/* terminate the token		*/
	return p1;
}

/***************************************************************************************/
/* character routines */
/***************************************************************************************/

/*
 *  Given a digit C return a non-zero value if the character is a whitespace character
 *   or 0 if it is not
 */
int  _ismbcspace(unsigned int c)
{
    //return (((c)>=0x824f) && ((c)<=0x8258));
    return isspace(c);
}

/*
 *  Given a digit C return a non-zero value if the character is a decimal digit
 *   or 0 if it is not
 */
int  _ismbcdigit(unsigned int c)
{
    //return (((c)>=0x824f) && ((c)<=0x8258));
    return isdigit(c);
}

int _ismbcupper(unsigned int s)
{
	//return ( (((s)>=0x8260)&& ((s)<=0x8279)));
   return 	isupper(s);
}

int _ismbclower(unsigned int s)
{
//	return ( (((s)>=0x8281) && ((s)<=0x829a)) );
   return islower(s);
}

int _ismbcalpha(unsigned int s)
{
	return (_ismbclower(s)||_ismbcupper(s));
}

#endif

