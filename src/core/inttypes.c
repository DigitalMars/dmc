
//_ inttypes.c
// Copyright (C) 2001 by Digital Mars
// All Rights Reserved
// Written by Walter Bright
// www.digitalmars.com

#include	<ctype.h>
#include	<errno.h>
#include	<stdlib.h>
#include 	<limits.h>
#include	<inttypes.h>

intmax_t imaxabs(intmax_t i)
{
#if __LONGLONG
    return llabs(i);
#else
    return labs(i);
#endif
}

imaxdiv_t imaxdiv(intmax_t numer, intmax_t denom)
{
    imaxdiv_t result;

    result.quot = numer / denom;
    result.rem = numer % denom;
    return result;
}

intmax_t strtoimax(const char * __restrict nptr,
		char ** __restrict endptr, int base)
{
#if __LONGLONG
    return strtoll(nptr, endptr, base);
#else
    return strtol(nptr, endptr, base);
#endif
}

uintmax_t strtoumax(const char * __restrict nptr,
		char ** __restrict endptr, int base)
{
#if __LONGLONG
    return strtoull(nptr, endptr, base);
#else
    return strtoul(nptr, endptr, base);
#endif
}

#if _WIN32

intmax_t wcstoimax(const wchar_t * __restrict nptr,
		wchar_t ** __restrict endptr, int base)
{
#if __LONGLONG
    return wcstoll(nptr, endptr, base);
#else
    return wcstol(nptr, endptr, base);
#endif
}

uintmax_t wcstoumax(const wchar_t * __restrict nptr,
		wchar_t ** __restrict endptr, int base)
{
#if __LONGLONG
    return wcstoull(nptr, endptr, base);
#else
    return wcstoul(nptr, endptr, base);
#endif
}

#endif
