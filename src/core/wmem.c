
#include <string.h>
#include <wchar.h>



wchar_t *wmemcpy(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, size_t n)
{
    return memcpy(s1, s2, n * sizeof(wchar_t));
}


wchar_t *wmemmove(wchar_t * __RESTRICT s1, const wchar_t * __RESTRICT s2, size_t n)
{
    return memmove(s1, s2, n * sizeof(wchar_t));
}


int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n)
{
    int i;

    while (n--)
    {
	i = *s1 - *s2;
	if (i)
	    break;
	s1++;
	s2++;
    }
    return i;
}


wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n)
{
    while (n--)
    {
	if (*s == c)
	    return (wchar_t *)s;
	s++;
    }
    return NULL;
}


wchar_t *wmemset(wchar_t *s, wchar_t c, size_t n)
{
    size_t i;

    for (i = 0; i < n; i++)
	s[i] = c;
    return s;
}
