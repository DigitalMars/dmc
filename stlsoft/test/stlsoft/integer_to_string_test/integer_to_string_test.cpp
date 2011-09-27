/* /////////////////////////////////////////////////////////////////////////////
 * File:        integer_to_string_test.cpp
 *
 * Purpose:
 *
 * Created:
 * Updated:     22nd April 2004
 *
 * Author:      Matthew Wilson
 *
 * Copyright:   Synesis Software Pty Ltd, (c) 2002. All rights reserved.
 *
 * Home:        www.stlsoft.org
 *
 * ////////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////////
 * integer_to_string.cpp
 *
 * Testing policies
 * ----------------
 *
 *  1. straight conversion                          integer_to_string
 *  2. 1 conversion into a string                   integer_to_string & strcpy
 *  3. 5 conversions into a single string           integer_to_string & strcpy
 *  4. 5 conversions into a single string           integer_to_string & strcpy & cch
 *  5. 5 conversions embedded in text               integer_to_string & strcpy
 *  6. 5 conversions embedded in text               integer_to_string & strcpy & cch
 *
 *     for both signed and unsigned
 *     for different sized integers
 */

#ifdef __DMC__
# if __DMC__ < 0x0829
#  error "This file doesn't correctly compile with versions of Digital Mars C/C++ < 8.29"
# endif /* __DMC__ < 0x0829 */
#endif /* __DMC__ */

#include <stdio.h>
#include <string.h>

#include <stlsoft.h>

//#define	__STLSOFT_OPTIMISE_64BIT

#undef __STLSOFT_CF_NEGATIVE_MODULUS_POSITIVE_GIVES_NEGATIVE_RESULT

stlsoft_ns_using(ss_sint8_t)
stlsoft_ns_using(ss_uint8_t)
stlsoft_ns_using(ss_sint16_t)
stlsoft_ns_using(ss_uint16_t)
stlsoft_ns_using(ss_sint32_t)
stlsoft_ns_using(ss_uint32_t)
stlsoft_ns_using(ss_sint64_t)
stlsoft_ns_using(ss_uint64_t)
stlsoft_ns_using(ss_char_a_t)
stlsoft_ns_using(ss_size_t)

#include <stlsoft_limit_traits.h>

stlsoft_ns_using(limit_traits)

#include <stlsoft_malloc_allocator.h>
#include <stlsoft_sign_traits.h>

stlsoft_ns_using(sign_traits)

#include <stlsoft_integer_to_string.h>

stlsoft_ns_using(integer_to_string)


#if defined(_MSC_VER) && \
	_MSC_VER < 1100

#include <windows.h>
int main(int /* argc */, char * /* argv */[])
{
	char	sz[41];
	int		i;

	for(i = 0; i < 100; ++i)
	{
		ss_sint32_t	x	=	::GetTickCount() ^ (::GetTickCount() << (i % 15));
#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
        const char  *s  =   integer_to_string(sz, x);
#else
        const char  *s  =   integer_to_string(sz, stlsoft_num_elements(sz), x);
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */

         printf("%d: %s\n", x, s);
    }

	return 0;
}
#else

#if defined(_Windows) || \
    defined(_WINDOWS) || \
    defined(WIN32) || \
    defined(_WIN32) || \
    defined(__WIN32__) || \
    defined(WIN64) || \
    defined(_WIN64) || \
    defined(__WIN64__) || \
    defined(_DOS) || \
    defined(__MSDOS__)

#include <winstl_performance_counter.h>

winstl_ns_using(performance_counter)

#else

#include <unixstl_performance_counter.h>

unixstl_ns_using(performance_counter)

#endif /* WIN32 */

////////////////////////////////////////////////////////////////////////////////
// Constants and definitions

/* long long printf format sequence */

#if defined(__BORLANDC__) || \
    defined(_INTEL_COMPILER) || \
    defined(_MSC_VER)
# define _sint64_fmt    "%I64d"
# define _uint64_fmt    "%I64u"
#elif defined(__DMC__) || \
      defined(__GNUC__) || \
      defined(__MWERKS__)
# define _sint64_fmt    "%lld"
# define _uint64_fmt    "%llu"
#else
# error Further compiler discrimination is required
#endif /* compiler */


#ifdef _DEBUG
const int ITERATIONS    =   100000;
#else
const int ITERATIONS    =   1000000;
#endif /* _DEBUG */

////////////////////////////////////////////////////////////////////////////////
// Typedefs

template <class T>
struct format_traits;

#ifdef __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX
template <>
#endif /* __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX */
struct format_traits<ss_sint8_t>
{
    typedef ss_sint8_t  value_type;

    enum { c = 's' };
    static const char *fmt1() { return "%d"; }
    static const char *fmt3() { return "%d%d%d%d%d"; }
    static const char *fmt5() { return "string0:%dstring1:%dstring2:%dstring3:%dstring4:%d"; }
    static value_type minimum() { return stlsoft_ns_qual(limit_traits)<ss_sint8_t>::minimum(); }
};

#ifdef __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX
template <>
#endif /* __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX */
struct format_traits<ss_uint8_t>
{
    typedef ss_uint8_t  value_type;

    enum { c = 'u' };
    static const char *fmt1() { return "%u"; }
    static const char *fmt3() { return "%u%u%u%u%u"; }
    static const char *fmt5() { return "string0:%ustring1:%ustring2:%ustring3:%ustring4:%u"; }
    static value_type minimum() { return static_cast<value_type>(-1); }
};

#ifdef __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX
template <>
#endif /* __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX */
struct format_traits<ss_sint16_t>
{
    typedef ss_sint16_t value_type;

    enum { c = 's' };
    static const char *fmt1() { return "%d"; }
    static const char *fmt3() { return "%d%d%d%d%d"; }
    static const char *fmt5() { return "string0:%dstring1:%dstring2:%dstring3:%dstring4:%d"; }
    static value_type minimum() { return stlsoft_ns_qual(limit_traits)<value_type>::minimum(); }
};

#ifdef __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX
template <>
#endif /* __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX */
struct format_traits<ss_uint16_t>
{
    typedef ss_uint16_t value_type;

    enum { c = 'u' };
    static const char *fmt1() { return "%u"; }
    static const char *fmt3() { return "%u%u%u%u%u"; }
    static const char *fmt5() { return "string0:%ustring1:%ustring2:%ustring3:%ustring4:%u"; }
    static value_type minimum() { return static_cast<value_type>(-1); }
};

#ifdef __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX
template <>
#endif /* __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX */
struct format_traits<ss_sint32_t>
{
    typedef ss_sint32_t value_type;

    enum { c = 's' };
    static const char *fmt1() { return "%ld"; }
    static const char *fmt3() { return "%ld%ld%ld%ld%ld"; }
    static const char *fmt5() { return "string0:%ldstring1:%ldstring2:%ldstring3:%ldstring4:%ld"; }
    static value_type minimum() { return stlsoft_ns_qual(limit_traits)<value_type>::minimum(); }
};

#ifdef __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX
template <>
#endif /* __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX */
struct format_traits<ss_uint32_t>
{
    typedef ss_uint32_t value_type;

    enum { c = 'u' };
    static const char *fmt1() { return "%lu"; }
    static const char *fmt3() { return "%lu%lu%lu%lu%lu"; }
    static const char *fmt5() { return "string0:%lustring1:%lustring2:%lustring3:%lustring4:%lu"; }
    static value_type minimum() { return (value_type)(-1 - ITERATIONS); }
};

#ifdef __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX
template <>
#endif /* __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX */
struct format_traits<ss_sint64_t>
{
    typedef ss_sint64_t value_type;

    enum { c = 's' };
    static const char *fmt1() { return _sint64_fmt; }
    static const char *fmt3() { return _sint64_fmt _sint64_fmt _sint64_fmt _sint64_fmt _sint64_fmt; }
    static const char *fmt5() { return "string0:" _sint64_fmt "string1:" _sint64_fmt "string2:" _sint64_fmt "string3:" _sint64_fmt "string4:" _sint64_fmt; }
    static value_type minimum() { return stlsoft_ns_qual(limit_traits)<value_type>::minimum(); }
};

#ifdef __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX
template <>
#endif /* __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX */
struct format_traits<ss_uint64_t>
{
    typedef ss_uint64_t value_type;

    enum { c = 'u' };
    static const char *fmt1() { return _uint64_fmt; }
    static const char *fmt3() { return _uint64_fmt _uint64_fmt _uint64_fmt _uint64_fmt _uint64_fmt; }
    static const char *fmt5() { return "string0:" _uint64_fmt "string1:" _uint64_fmt "string2:" _uint64_fmt "string3:" _uint64_fmt "string4:" _uint64_fmt; }
    static value_type minimum() { return (value_type)(-1 - ITERATIONS); }
};

////////////////////////////////////////////////////////////////////////////////
// Functions

inline double calc_percent(const performance_counter::interval_type &i2str_time, const performance_counter::interval_type &printf_time)
{
    if(printf_time == 0)
    {
        return 0.0;
    }
    else
    {
        return ((double)static_cast<ss_sint64_t>(i2str_time) * 100.0) / static_cast<ss_sint64_t>(printf_time);
    }
}

template <class T>
static void func1(const T &)
{
    typedef T                   integer_type;
    typedef limit_traits<T>     limit_traits_type;
    typedef sign_traits<T>      sign_traits_type;
    typedef format_traits<T>    format_traits_type;

    fprintf(stdout, "\n");

    performance_counter                 counter;
    int                                 i;
    char                                result[201];
    performance_counter::interval_type  printf_time;
    performance_counter::interval_type  i2str_time;

    char    szFmt[101];

    strcpy(szFmt, format_traits_type::fmt1());

    // verification
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();
        char            sz1[201];
        char            sz2[201];

        for(j = 0; j < ITERATIONS; ++j, ++val)
        {
            sprintf(sz1, szFmt, val);

#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
            const char  *s  =   integer_to_string(sz2, val);
#else
            const char  *s  =   integer_to_string(sz2, stlsoft_num_elements(result), val);
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */

            if(strcmp(sz1, s) != 0)
            {
                printf("bad(%d): \"%s\" \"%s\"\n", j, sz1, s);
                exit(1);
            }
        }
    }

    // printf

    for(i = 0; i < 2; ++i)
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();

        for(counter.start(), j = 0; j < ITERATIONS; ++j, ++val)
        {
            sprintf(result, szFmt, val);
        }
        counter.stop();
    }
    printf_time =   counter.get_milliseconds();
    fprintf(stdout, "1, %c%2d: printf :\t" _sint64_fmt "ms\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        printf_time);

    // integer_to_string

    for(i = 0; i < 2; ++i)
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();

        for(counter.start(), j = 0; j < ITERATIONS; ++j, ++val)
        {
#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
            integer_to_string(result, val);
#else
            integer_to_string(result, stlsoft_num_elements(result), val);
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */
        }
        counter.stop();
    }
    i2str_time  =   counter.get_milliseconds();
    fprintf(stdout, "1, %c%2d: i2str  :\t" _sint64_fmt "ms\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        i2str_time);

    fprintf(stdout, "1, %c%2d: rel    :\t\t%4.2f%%\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        calc_percent(i2str_time, printf_time));
}

template <class T>
static void func2(const T &)
{
    typedef T                   integer_type;
    typedef limit_traits<T>     limit_traits_type;
    typedef sign_traits<T>      sign_traits_type;
    typedef format_traits<T>    format_traits_type;

    fprintf(stdout, "\n");

    performance_counter                 counter;
    int                                 i;
    char                                _result[201];
    char                                result[201];
    performance_counter::interval_type  printf_time;
    performance_counter::interval_type  i2str_time;

    char    szFmt[101];

    strcpy(szFmt, format_traits_type::fmt1());

    // verification
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();
        char            sz1[201];
        char            sz2[201];
        ss_size_t       cchRes;

        for(j = 0; j < ITERATIONS; ++j, ++val)
        {
            sprintf(sz1, szFmt, val);

#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
            const char  *s  =   integer_to_string(sz2, val, cchRes);
#else
            const char  *s  =   integer_to_string(sz2, stlsoft_num_elements(result), val, cchRes);
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */

            if(strcmp(sz1, s) != 0)
            {
                printf("bad(%d): \"%s\" \"%s\"\n", j, sz1, s);
                exit(1);
            }
        }
    }

    // printf

    for(i = 0; i < 2; ++i)
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();

        for(counter.start(), j = 0; j < ITERATIONS; ++j, ++val)
        {
            sprintf(result, szFmt, val);
        }
        counter.stop();
    }
    printf_time =   counter.get_milliseconds();
    fprintf(stdout, "2, %c%2d: printf :\t" _sint64_fmt "ms\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        printf_time);

    // integer_to_string

    for(i = 0; i < 2; ++i)
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();

        for(counter.start(), j = 0; j < ITERATIONS; ++j, ++val)
        {
#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
            strcpy(result, integer_to_string(_result, val));
#else
            strcpy(result, integer_to_string(_result, stlsoft_num_elements(_result), val));
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */
        }
        counter.stop();
    }
    i2str_time  =   counter.get_milliseconds();
    fprintf(stdout, "2, %c%2d: i2str  :\t" _sint64_fmt "ms\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        i2str_time);

    fprintf(stdout, "2, %c%2d: rel    :\t\t%4.2f%%\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        calc_percent(i2str_time, printf_time));
}

template <class T>
static void func3(const T &)
{
    typedef T                   integer_type;
    typedef limit_traits<T>     limit_traits_type;
    typedef sign_traits<T>      sign_traits_type;
    typedef format_traits<T>    format_traits_type;

    fprintf(stdout, "\n");

    performance_counter                 counter;
    int                                 i;
    char                                _result[201];
    char                                result[201];
    performance_counter::interval_type  printf_time;
    performance_counter::interval_type  i2str_time;

    char    szFmt[101];

    strcpy(szFmt, format_traits_type::fmt3());

    // verification
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();
        char            sz1[201];
        char            sz2[201];

        for(j = 0; j < ITERATIONS; ++j, ++val)
        {
            sprintf(sz1, szFmt, val, val, val, val, val);

#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
            strcpy(sz2, integer_to_string(_result, val));
            strcat(sz2, integer_to_string(_result, val));
            strcat(sz2, integer_to_string(_result, val));
            strcat(sz2, integer_to_string(_result, val));
            strcat(sz2, integer_to_string(_result, val));
#else
            strcpy(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val));
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */

            if(strcmp(sz1, sz2) != 0)
            {
                printf("bad(%d): \"%s\" \"%s\"\n", j, sz1, sz2);
                exit(1);
            }
        }
    }

    // printf

    for(i = 0; i < 2; ++i)
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();

        for(counter.start(), j = 0; j < ITERATIONS; ++j, ++val)
        {
            sprintf(result, szFmt, val, val, val, val, val);
        }
        counter.stop();
    }
    printf_time =   counter.get_milliseconds();
    fprintf(stdout, "3, %c%2d: printf :\t" _sint64_fmt "ms\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        printf_time);

    // integer_to_string

    for(i = 0; i < 2; ++i)
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();

        for(counter.start(), j = 0; j < ITERATIONS; ++j, ++val)
        {
#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
            strcpy(result, integer_to_string(_result, val));
            strcat(result, integer_to_string(_result, val));
            strcat(result, integer_to_string(_result, val));
            strcat(result, integer_to_string(_result, val));
            strcat(result, integer_to_string(_result, val));
#else
            strcpy(result, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(result, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(result, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(result, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(result, integer_to_string(_result, stlsoft_num_elements(_result), val));
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */
        }
        counter.stop();
    }
    i2str_time  =   counter.get_milliseconds();
    fprintf(stdout, "3, %c%2d: i2str  :\t" _sint64_fmt "ms\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        i2str_time);

    fprintf(stdout, "3, %c%2d: rel    :\t\t%4.2f%%\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        calc_percent(i2str_time, printf_time));
}

template <class T>
static void func4(const T &)
{
    typedef T                   integer_type;
    typedef limit_traits<T>     limit_traits_type;
    typedef sign_traits<T>      sign_traits_type;
    typedef format_traits<T>    format_traits_type;

    fprintf(stdout, "\n");

    performance_counter                 counter;
    int                                 i;
    char                                _result[201];
    char                                result[201];
    performance_counter::interval_type  printf_time;
    performance_counter::interval_type  i2str_time;

    char    szFmt[101];

    strcpy(szFmt, format_traits_type::fmt3());

    // verification
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();
        char            sz1[201];
        char            sz2[201];
        ss_size_t       cchRes;

        for(j = 0; j < ITERATIONS; ++j, ++val)
        {
            sprintf(sz1, szFmt, val, val, val, val, val);

#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
            strcpy(sz2, integer_to_string(_result, val, cchRes));
            strcat(sz2, integer_to_string(_result, val, cchRes));
            strcat(sz2, integer_to_string(_result, val, cchRes));
            strcat(sz2, integer_to_string(_result, val, cchRes));
            strcat(sz2, integer_to_string(_result, val, cchRes));
#else
            strcpy(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            strcat(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            strcat(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            strcat(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            strcat(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */

            if(strcmp(sz1, sz2) != 0)
            {
                printf("bad(%d): \"%s\" \"%s\"\n", j, sz1, sz2);
                exit(1);
            }
        }
    }

    // printf

    for(i = 0; i < 2; ++i)
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();

        for(counter.start(), j = 0; j < ITERATIONS; ++j, ++val)
        {
            sprintf(result, szFmt, val, val, val, val, val);
        }
        counter.stop();
    }
    printf_time =   counter.get_milliseconds();
    fprintf(stdout, "4, %c%2d: printf :\t" _sint64_fmt "ms\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        printf_time);

    // integer_to_string

    for(i = 0; i < 2; ++i)
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();

        for(counter.start(), j = 0; j < ITERATIONS; ++j, ++val)
        {
            ss_size_t   cchRes;
            ss_char_a_t *dest;

#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
            dest = strcpy(result, integer_to_string(_result, val, cchRes));
            dest = strcpy(dest + cchRes, integer_to_string(_result, val, cchRes));
            dest = strcpy(dest + cchRes, integer_to_string(_result, val, cchRes));
            dest = strcpy(dest + cchRes, integer_to_string(_result, val, cchRes));
            dest = strcpy(dest + cchRes, integer_to_string(_result, val, cchRes));
#else
            dest = strcpy(result, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            dest = strcpy(dest + cchRes, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            dest = strcpy(dest + cchRes, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            dest = strcpy(dest + cchRes, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            dest = strcpy(dest + cchRes, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */
            dest;
        }
        counter.stop();
    }
    i2str_time  =   counter.get_milliseconds();
    fprintf(stdout, "4, %c%2d: i2str  :\t" _sint64_fmt "ms\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        i2str_time);

    fprintf(stdout, "4, %c%2d: rel    :\t\t%4.2f%%\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        calc_percent(i2str_time, printf_time));
}


template <class T>
static void func5(const T &)
{
    typedef T                   integer_type;
    typedef limit_traits<T>     limit_traits_type;
    typedef sign_traits<T>      sign_traits_type;
    typedef format_traits<T>    format_traits_type;

    fprintf(stdout, "\n");

    performance_counter                 counter;
    int                                 i;
    char                                _result[201];
    char                                result[201];
    performance_counter::interval_type  printf_time;
    performance_counter::interval_type  i2str_time;

    char    szFmt[101];

    strcpy(szFmt, format_traits_type::fmt5());

    // verification
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();
        char            sz1[201];
        char            sz2[201];

        for(j = 0; j < ITERATIONS; ++j, ++val)
        {
            sprintf(sz1, szFmt, val, val, val, val, val);

#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
            strcpy(sz2, "string0:");
            strcat(sz2, integer_to_string(_result, val));
            strcat(sz2, "string1:");
            strcat(sz2, integer_to_string(_result, val));
            strcat(sz2, "string2:");
            strcat(sz2, integer_to_string(_result, val));
            strcat(sz2, "string3:");
            strcat(sz2, integer_to_string(_result, val));
            strcat(sz2, "string4:");
            strcat(sz2, integer_to_string(_result, val));
#else
            strcpy(sz2, "string0:");
            strcat(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(sz2, "string1:");
            strcat(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(sz2, "string2:");
            strcat(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(sz2, "string3:");
            strcat(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(sz2, "string4:");
            strcat(sz2, integer_to_string(_result, stlsoft_num_elements(_result), val));
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */

            if(strcmp(sz1, sz2) != 0)
            {
                printf("bad(%d): \"%s\" \"%s\"\n", j, sz1, sz2);
                exit(1);
            }
        }
    }

    // printf

    for(i = 0; i < 2; ++i)
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();

        for(counter.start(), j = 0; j < ITERATIONS; ++j, ++val)
        {
            sprintf(result, szFmt, val, val, val, val, val);
        }
        counter.stop();
    }
    printf_time =   counter.get_milliseconds();
    fprintf(stdout, "5, %c%2d: printf :\t" _sint64_fmt "ms\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        printf_time);

    // integer_to_string

    for(i = 0; i < 2; ++i)
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();

        for(counter.start(), j = 0; j < ITERATIONS; ++j, ++val)
        {
#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
            strcpy(result, "string0:");
            strcat(result, integer_to_string(_result, val));
            strcat(result, "string1:");
            strcat(result, integer_to_string(_result, val));
            strcat(result, "string2:");
            strcat(result, integer_to_string(_result, val));
            strcat(result, "string3:");
            strcat(result, integer_to_string(_result, val));
            strcat(result, "string4:");
            strcat(result, integer_to_string(_result, val));
#else
            strcpy(result, "string0:");
            strcat(result, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(result, "string1:");
            strcat(result, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(result, "string2:");
            strcat(result, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(result, "string3:");
            strcat(result, integer_to_string(_result, stlsoft_num_elements(_result), val));
            strcat(result, "string4:");
            strcat(result, integer_to_string(_result, stlsoft_num_elements(_result), val));
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */
        }
        counter.stop();
    }
    i2str_time  =   counter.get_milliseconds();
    fprintf(stdout, "5, %c%2d: i2str  :\t" _sint64_fmt "ms\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        i2str_time);

    fprintf(stdout, "5, %c%2d: rel    :\t\t%4.2f%%\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        calc_percent(i2str_time, printf_time));
}

template <class T>
static void func6(const T &)
{
    typedef T                   integer_type;
    typedef limit_traits<T>     limit_traits_type;
    typedef sign_traits<T>      sign_traits_type;
    typedef format_traits<T>    format_traits_type;

    fprintf(stdout, "\n");

    performance_counter                 counter;
    int                                 i;
    char                                _result[201];
    char                                result[201];
    performance_counter::interval_type  printf_time;
    performance_counter::interval_type  i2str_time;

    char    szFmt[101];

    strcpy(szFmt, format_traits_type::fmt5());

    // verification
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();
        char            sz1[201];
        char            sz2[201];

        for(j = 0; j < ITERATIONS; ++j, ++val)
        {
            ss_size_t   cchRes;
            ss_char_a_t *dest;

            sprintf(sz1, szFmt, val, val, val, val, val);

#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
            dest = strcpy(sz2, "string0:");
            dest = strcpy(dest + 8, integer_to_string(_result, val, cchRes));
            dest = strcpy(dest + cchRes, "string1:");
            dest = strcpy(dest + 8, integer_to_string(_result, val, cchRes));
            dest = strcpy(dest + cchRes, "string2:");
            dest = strcpy(dest + 8, integer_to_string(_result, val, cchRes));
            dest = strcpy(dest + cchRes, "string3:");
            dest = strcpy(dest + 8, integer_to_string(_result, val, cchRes));
            dest = strcpy(dest + cchRes, "string4:");
            dest = strcpy(dest + 8, integer_to_string(_result, val, cchRes));
            (void)dest;
#else
            dest = strcpy(sz2, "string0:");
            dest = strcpy(dest + 8, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            dest = strcpy(dest + cchRes, "string1:");
            dest = strcpy(dest + 8, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            dest = strcpy(dest + cchRes, "string2:");
            dest = strcpy(dest + 8, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            dest = strcpy(dest + cchRes, "string3:");
            dest = strcpy(dest + 8, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            dest = strcpy(dest + cchRes, "string4:");
            dest = strcpy(dest + 8, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            (void)dest;
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */

            if(strcmp(sz1, sz2) != 0)
            {
                printf("bad(%d): \"%s\" \"%s\"\n", j, sz1, sz2);
                exit(1);
            }
        }
    }

    // printf

    for(i = 0; i < 2; ++i)
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();

        for(counter.start(), j = 0; j < ITERATIONS; ++j, ++val)
        {
            sprintf(result, szFmt, val, val, val, val, val);
        }
        counter.stop();
    }
    printf_time =   counter.get_milliseconds();
    fprintf(stdout, "6, %c%2d: printf :\t" _sint64_fmt "ms\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        printf_time);

    // integer_to_string

    for(i = 0; i < 2; ++i)
    {
        int             j;
        integer_type    val =   format_traits_type::minimum();

        for(counter.start(), j = 0; j < ITERATIONS; ++j, ++val)
        {
            ss_size_t   cchRes;
            ss_char_a_t *dest;

#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
            dest = strcpy(result, "string0:");
            dest = strcpy(dest + 8, integer_to_string(_result, val, cchRes));
            dest = strcpy(dest + cchRes, "string1:");
            dest = strcpy(dest + 8, integer_to_string(_result, val, cchRes));
            dest = strcpy(dest + cchRes, "string2:");
            dest = strcpy(dest + 8, integer_to_string(_result, val, cchRes));
            dest = strcpy(dest + cchRes, "string3:");
            dest = strcpy(dest + 8, integer_to_string(_result, val, cchRes));
            dest = strcpy(dest + cchRes, "string4:");
            dest = strcpy(dest + 8, integer_to_string(_result, val, cchRes));
            (void)dest;
#else
            dest = strcpy(result, "string0:");
            dest = strcpy(dest + 8, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            dest = strcpy(dest + cchRes, "string1:");
            dest = strcpy(dest + 8, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            dest = strcpy(dest + cchRes, "string2:");
            dest = strcpy(dest + 8, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            dest = strcpy(dest + cchRes, "string3:");
            dest = strcpy(dest + 8, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            dest = strcpy(dest + cchRes, "string4:");
            dest = strcpy(dest + 8, integer_to_string(_result, stlsoft_num_elements(_result), val, cchRes));
            (void)dest;
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */
        }
        counter.stop();
    }
    i2str_time  =   counter.get_milliseconds();
    fprintf(stdout, "6, %c%2d: i2str  :\t" _sint64_fmt "ms\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        i2str_time);

    fprintf(stdout, "6, %c%2d: rel    :\t\t%4.2f%%\n",
                        format_traits_type::c,
                        sign_traits_type::bits,
                        calc_percent(i2str_time, printf_time));
}

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char ** /* argv */)
{
    ////////////////////////////////////
    // 1. straight conversion

#ifdef __STLSOFT_CF_NEGATIVE_MODULUS_POSITIVE_GIVES_NEGATIVE_RESULT
	puts("signed-integer conversion direct");
#else
	puts("signed-integer conversion uses unsigned-integer conversion");
#endif // __STLSOFT_CF_NEGATIVE_MODULUS_POSITIVE_GIVES_NEGATIVE_RESULT


    func1(ss_sint8_t());
    func1(ss_uint8_t());
    func1(ss_sint16_t());
    func1(ss_uint16_t());
    func1(ss_sint32_t());
    func1(ss_uint32_t());
    func1(ss_sint64_t());
    func1(ss_uint64_t());

#ifdef __SYNSOFT_DBS_DEBUG
    func2(ss_sint8_t());
    func2(ss_uint8_t());
    func2(ss_sint16_t());
    func2(ss_uint16_t());
    func2(ss_sint32_t());
    func2(ss_uint32_t());
    func2(ss_sint64_t());
    func2(ss_uint64_t());

    func3(ss_sint8_t());
    func3(ss_uint8_t());
    func3(ss_sint16_t());
    func3(ss_uint16_t());
    func3(ss_sint32_t());
    func3(ss_uint32_t());
    func3(ss_sint64_t());
    func3(ss_uint64_t());

    func4(ss_sint8_t());
    func4(ss_uint8_t());
    func4(ss_sint16_t());
    func4(ss_uint16_t());
    func4(ss_sint32_t());
    func4(ss_uint32_t());
    func4(ss_sint64_t());
    func4(ss_uint64_t());

    func5(ss_sint8_t());
    func5(ss_uint8_t());
    func5(ss_sint16_t());
    func5(ss_uint16_t());
    func5(ss_sint32_t());
    func5(ss_uint32_t());
    func5(ss_sint64_t());
    func5(ss_uint64_t());
#endif // !__SYNSOFT_DBS_DEBUG

#if 0
    func6(ss_sint8_t());
    func6(ss_uint8_t());
    func6(ss_sint16_t());
    func6(ss_uint16_t());
    func6(ss_sint32_t());
    func6(ss_uint32_t());
    func6(ss_sint64_t());
    func6(ss_uint64_t());
#endif /* 0 */

    return 0;
}

#endif /* _MSC_VER < 1100 */

////////////////////////////////////////////////////////////////////////////////
