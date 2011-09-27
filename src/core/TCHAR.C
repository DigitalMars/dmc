/* Copyright (C) 1994 by Digital Mars
 * All  Rights Reserved
 */
 
 /*
  *  Simple mappings from _tcs routines to mbcs routines
  */

#define _NO_INLINING 1  // Turn off inlining in tchar.h
#include <tchar.h>
#include <mbstring.h>

#ifdef _tcschr
#undef _tcschr
#endif
char * _tcschr(const char * s, unsigned int i)
{
    return (char *)_mbschr((const unsigned char *)s,i);
}


#ifdef _tcscmp
#undef _tcscmp
#endif
int _tcscmp(const char *s1, const char *s2)
{
    return  _mbscmp((const unsigned char *)s1,(const unsigned char *)s2);
}


#ifdef _tcsncmp
#undef _tcsncmp
#endif
int _tcsncmp(const char *s1, const char *s2, size_t i)
{ 
    return _mbsnbcmp((const unsigned char *)s1,(const unsigned char *)s2,i);
}
 
#ifdef _tcscspn
#undef _tcscspn
#endif
size_t _tcscspn(const char *s1, const char *s2)
{
    return _mbscspn((const unsigned char *)s1,(const unsigned char *)s2);
}


#ifdef _tcsrev
#undef _tcsrev
#endif
char *_tcsrev(char *s)
{
     return (char *)_mbsrev((unsigned char *)s);
}


#ifdef _tcsicmp
#undef _tcsicmp
#endif
int _tcsicmp(const char *s1, const char *s2)
{
     return _mbsicmp((const unsigned char *)s1,(const unsigned char *)s2);
}


#ifdef _tcsnicmp
#undef _tcsnicmp
#endif
int _tcsnicmp(const char *s1, const char *s2, size_t i)
{
    return _mbsnbicmp((const unsigned char *)s1,(const unsigned char *)s2,i);
}

#ifdef _tcsinc
#undef _tcsinc
#endif
char *_tcsinc(const char *s)
{
     return (char *)_mbsinc((const unsigned char *)s);
}

#ifdef _tcsninc
#undef _tcsninc
#endif
char *_tcsninc(const char *s, size_t n)
{
    return (char *)_mbsninc((const unsigned char *)s,n);
}

#ifdef _tcsdec
#undef _tcsdec
#endif
char * _tcsdec(const char *s1, const char *s2)
{
      return (char *)_mbsdec((const unsigned char *)s1, (const unsigned char *)s2);
}

#ifdef _tclen
#undef _tclen
#endif
size_t _tclen(const char *s)
{
   return _mbclen((const unsigned char *)s);
}

#ifdef _tcsstr
#undef _tcsstr
#endif
char * _tcsstr(const char *s1, const char *s2)
{
    return (char *)_mbsstr((const unsigned char *)s1,(const unsigned char *)s2);
}


#ifdef _tcsspn
#undef _tcsspn
#endif
size_t _tcsspn(const char *s1, const char *s2)
{
    return _mbsspn((const unsigned char *)s1,(const unsigned char *)s2);
}


#ifdef _tcspbrk
#undef _tcspbrk
#endif
char * _tcspbrk(const char *s1, const char *s2)
{
    return (char *)_mbspbrk((const unsigned char *)s1,(const unsigned char *)s2);
}


#ifdef _tcsrchr
#undef _tcsrchr
#endif
char * _tcsrchr(const char *s, int i)
{
    return (char *)_mbsrchr((const unsigned char *)s,i);
}


#ifdef _tcsncpy
#undef _tcsncpy
#endif
char *  _tcsncpy(char *s1, const char *s2, size_t i)
{
    return (char *)_mbsnbcpy((unsigned char *)s1,(const unsigned char *)s2,i);
}
    
#ifdef _tcstok
#undef _tcstok
#endif
char * _tcstok(char *s1, const char *s2)
{
   return (char *)_mbstok((unsigned char *)s1,(const unsigned char *)s2);
}


#ifdef _tcsclen
#undef _tcsclen
#endif
size_t _tcsclen(const char * _s1) 
{
    return _mbslen((const unsigned char *)_s1);
}

#ifdef _tcsnccmp
#undef _tcsnccmp
#endif
int _tcsnccmp(const char * _s1,const char * _s2,size_t _n) 
{
    return _mbsncmp((const unsigned char *)_s1,(const unsigned char *)_s2,_n);
}


#ifdef _tcsnextc
#undef _tcsnextc
#endif
unsigned int _tcsnextc(const char * _s1) 
{
    unsigned int _n=0; 
    
    if (_ismbblead((unsigned int)(unsigned char *)*_s1))
        _n=((unsigned int)*_s1++)<<8;
        
    _n+=(unsigned int)*_s1; 
    return(_n);
}
