/* /////////////////////////////////////////////////////////////////////////////
 * File:        whereis.cpp
 *
 * Purpose:     Implementation file for the Synesis Software whereis utility
 *
 * Created:     19th January 1996
 * Updated:     16th August 2004
 *
 * Author:      Matthew Wilson, Synesis Software Pty Ltd.
 *
 *              The timestr() function and the verbose formatting in trace_file
 *              used by kind permission of Walter Bright and Digital Mars.
 *
 * License:     (Licensed under the Synesis Software Standard Public License)
 *
 *              Copyright (C) 1999-2004, Synesis Software Pty Ltd.
 *              Copyright (C) 1987-2004, Digital Mars.
 *
 *              All rights reserved.
 *
 *              www:        http://www.synesis.com.au
 *                          http://www.synesis.com.au/software
 *
 *                          http://www.digitalmars.com/
 *
 *              email:      software@synesis.com.au
 *
 *                          contact@digitalmars.com
 *
 *              Redistribution and use in source and binary forms, with or
 *              without modification, are permitted provided that the following
 *              conditions are met:
 *
 *              (i) Redistributions of source code must retain the above
 *              copyright notice and contact information, this list of
 *              conditions and the following disclaimer.
 *
 *              (ii) Redistributions in binary form must reproduce the above
 *              copyright notice and contact information, this list of
 *              conditions and the following disclaimer in the documentation
 *              and/or other materials provided with the distribution.
 *
 *              (iii) Any derived versions of this software (howsoever modified)
 *              remain the sole property of Synesis Software.
 *
 *              (iv) Any derived versions of this software (howsoever modified)
 *              remain subject to all these conditions.
 *
 *              (v) Neither the name of Synesis Software nor the names of any
 *              subdivisions, employees or agents of Synesis Software, nor the
 *              names of any other contributors to this software may be used to
 *              endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              This source code is provided by Synesis Software "as is" and any
 *              warranties, whether expressed or implied, including, but not
 *              limited to, the implied warranties of merchantability and
 *              fitness for a particular purpose are disclaimed. In no event
 *              shall the Synesis Software be liable for any direct, indirect,
 *              incidental, special, exemplary, or consequential damages
 *              (including, but not limited to, procurement of substitute goods
 *              or services; loss of use, data, or profits; or business
 *              interruption) however caused and on any theory of liability,
 *              whether in contract, strict liability, or tort (including
 *              negligence or otherwise) arising in any way out of the use of
 *              this software, even if advised of the possibility of such
 *              damage.
 *
 * ////////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#include <stdio.h>

#include <stlsoft.h>

stlsoft_ns_using(ss_uint_t)
stlsoft_ns_using(ss_sint64_t)
stlsoft_ns_using(ss_bool_t)

/* Regrettably, something in this program is too complex for early versions of
 * many compilers that the STLSoft libraries support. With some it is a compiler
 * issue, with others it either crashes or produces erroneous output. Thus, the
 * following restrictions are mandated:
 */
#if defined(__STLSOFT_COMPILER_IS_BORLAND)
# if __BORLANDC__ < 0x0560
#  error whereis.cpp cannot be successfully built by Borland C/C++ prior to version 5.6
# endif /* __BORLANDC__ < 0x0560 */
#elif defined(__STLSOFT_COMPILER_IS_DMC)
# if __DMC__ < 0x0832
#  error whereis.cpp cannot be successfully built by Digital Mars C/C++ prior to version 8.32
# endif /* __DMC__ < 0x0832 */
#elif defined(__STLSOFT_COMPILER_IS_GCC)
# if __GNUC__ < 3 && \
     __GNUC_MINOR__ < 95
#  error whereis.cpp cannot be successfully built by GNU C/C++ prior to version 6.0
# endif /* __GNUC__ < 3 && __GNUC_MINOR__ < 95 */
#elif defined(__STLSOFT_COMPILER_IS_INTEL)
# if __INTEL_COMPILER < 600
#  error whereis.cpp cannot be successfully built by Intel C/C++ prior to version 6.0
# endif /* __INTEL_COMPILER < 600 */
#elif defined(__STLSOFT_COMPILER_IS_MWERKS)
 /* No problems with Metrowerks */
#elif defined(__STLSOFT_COMPILER_IS_MSVC)
# if _MSC_VER < 1100
#  error whereis.cpp cannot be successfully built by Microsoft Visual C/C++ prior to version 5.0
# endif /* _MSC_VER < 1100 */
#else
# error Unrecognised compiler
#endif /* compiler */

/* /////////////////////////////////////////////////////////////////////////////
 * Warnings
 */

#if defined(__STLSOFT_COMPILER_IS_MSVC)
# pragma warning(disable : 4702)
#endif /* __STLSOFT_COMPILER_IS_INTEL */

/* ////////////////////////////////////////////////////////////////////////// */

#ifdef _SYNSOFT_INTERNAL_BUILD
# include "MLCompId.h"
#else /* ? _SYNSOFT_INTERNAL_BUILD */
# define _nameSynesisSoftware               "Synesis Software (Pty) Ltd"
# define _wwwSynesisSoftware_SystemTools    "http://synesis.com.au/systools.html"
# define _emailSynesisSoftware_SystemTools  "software@synesis.com.au"
#endif /* _SYNSOFT_INTERNAL_BUILD */

/* ////////////////////////////////////////////////////////////////////////// */

#include <stlsoft_auto_buffer.h>

stlsoft_ns_using(auto_buffer)

#include <stlsoft_limit_traits.h>

#include <stlsoft_malloc_allocator.h>

stlsoft_ns_using(malloc_allocator)

#include <stlsoft_string_access.h>

stlsoft_ns_using(c_str_ptr)

#include <stlsoft_simple_algorithms.h>

stlsoft_ns_using(remove_duplicates_from_unordered_sequence)

#include <stlsoft_string_tokeniser.h>

stlsoft_ns_using(string_tokeniser)

#if defined(UNIX) || \
    defined(unix) || \
    defined(__unix) || \
    defined(__unix__) || \
    (   defined(__xlC__) && \
        defined(_POWER) && \
        defined(_AIX))

/* UNIX includes */
# define WHEREIS_PLATFORM_IS_UNIX

# include <unixstl.h>

namespace platform_stl = unixstl;

# include <unixstl_file_path_buffer.h>

 typedef unixstl_ns_qual(file_path_buffer_a)		file_path_buffer;

# include <unixstl_findfile_sequence.h>

 unixstl_ns_using(findfile_sequence)

# include <unixstl_functionals.h>

 unixstl_ns_using(compare_path)

# include <unixstl_environment_variable.h>

 unixstl_ns_using(basic_environment_variable)

# include <unixstl_current_directory.h>

 unixstl_ns_using(basic_current_directory)

# include <unixstl_filesystem_traits.h>

 unixstl_ns_using(filesystem_traits)

# include <time.h>
# include <sys/types.h>
# include <sys/stat.h>

#elif defined(WIN32) || \
      defined(_WIN32)

/* Win32 includes */
# define WHEREIS_PLATFORM_IS_WIN32

# include <winstl.h>

namespace platform_stl = winstl;

# include <winstl_file_path_buffer.h>

 typedef winstl_ns_qual(file_path_buffer_a)			file_path_buffer;

# include <winstl_findfile_sequence.h>

 typedef winstl_ns_qual(findfile_sequence_a)		findfile_sequence;

# include <winstl_functionals.h>

 winstl_ns_using(compare_path)

# include <winstl_environment_variable.h>

 winstl_ns_using(basic_environment_variable)

# include <winstl_current_directory.h>

 winstl_ns_using(basic_current_directory)

# include <winstl_filesystem_traits.h>

 winstl_ns_using(filesystem_traits)

#else

# error Operating system not recognised

#endif /* operating system */
#include <algorithm>

stlsoft_ns_using_std(copy)
stlsoft_ns_using_std(for_each)
stlsoft_ns_using_std(remove_if)

#include <functional>

stlsoft_ns_using_std(unary_function)

#include <iterator>

stlsoft_ns_using_std(back_inserter)

#include <string>

stlsoft_ns_using_std(string)

#include <vector>

stlsoft_ns_using_std(vector)

#include <list>

stlsoft_ns_using_std(list)

#include <map>

stlsoft_ns_using_std(map)


#ifdef _SYNSOFT_INTERNAL_BUILD
# include <MRPathFn.h>
# ifdef WHEREIS_PLATFORM_IS_WIN32
#  define WHEREIS_USING_WIN32_VERSION_INFO
#  include <MWVerInf.h>
# endif /* platform */
#endif /* _SYNSOFT_INTERNAL_BUILD */

/* /////////////////////////////////////////////////////////////////////////////
 * Warnings
 */

#if defined(__STLSOFT_COMPILER_IS_INTEL)
# pragma warning(disable : 383)
# pragma warning(disable : 444)
# pragma warning(disable : 1419)
#endif /* __STLSOFT_COMPILER_IS_INTEL */

/* /////////////////////////////////////////////////////////////////////////////
 * Tracing
 */

//#define WHEREIS_TRACE

#if !defined(WHEREIS_TRACE) && \
    defined(_DEBUG)
# define WHEREIS_TRACE
#endif /* !WHEREIS_TRACE && _DEBUG */

/* /////////////////////////////////////////////////////////////////////////////
 * Version information
 *
 * Normally, Synesis Software components & tools are "versioned" by making use
 * of tool-generated header files, one of which - MBldHdr.h - defines symbolic
 * constants representing major, minor, revision and build numbers.
 *
 *
 * Since this file is to be available in a more public forum, including on the
 * Digital Mars site, these constants are merely defined here, rather than
 * encumber any developers who may wish to compile it with a lot of
 * Synesis-specific gunk.
 */

#ifdef _SYNSOFT_INTERNAL_BUILD
# include "MBldHdr.h"
# include "showver.h"

extern "C" const int  _mccVerHi   =   __SYV_MAJOR;
extern "C" const int  _mccVerLo   =   __SYV_MINOR;
extern "C" const int  _mccVerRev  =   __SYV_REVISION;
extern "C" const int  _mccBldNum  =   __SYV_BUILDNUMBER;

#else /* ? _SYNSOFT_INTERNAL_BUILD */
extern "C" const int  _mccVerHi   =   1;
extern "C" const int  _mccVerLo   =   12;
extern "C" const int  _mccVerRev  =   1;
extern "C" const int  _mccBldNum  =   52;
#endif /* _SYNSOFT_INTERNAL_BUILD */

# define    COMPANY_NAME    "Synesis Software"
# define    TOOL_NAME       "File Searching"
# define    EXE_NAME        "whereis"

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs
 */

typedef string_tokeniser<string, char>  tokeniser_string_t;
typedef vector<string>                  searchpath_sorted_t;
//typedef   list<string>                searchpath_sorted_t;
typedef map<string, unsigned>			extension_map_t;

#if defined(WHEREIS_PLATFORM_IS_WIN32)
typedef FILETIME                        whereis_time_t;
#elif defined(WHEREIS_PLATFORM_IS_UNIX)
typedef time_t                          whereis_time_t;
#else
# error Unknown platform
#endif /* platform */

/* /////////////////////////////////////////////////////////////////////////////
 * Enumerations
 */

    namespace Trim
    {
        enum Trim
        {
                full
            ,   fileOnly
            ,   fromSearchRoot
            ,   fromCurrentDirectory
        };
    } // namespace Trim

    namespace Flags
    {
        enum
        {
                verbose         =   0x00000001
            ,   showVersionInfo =   0x00000002
            ,   showFiles       =   0x00000004
            ,   showDirectories =   0x00000008
            ,   markDirectories =   0x00000010
        };

    } // namespace Flags

/* /////////////////////////////////////////////////////////////////////////////
 * Macros and definitions
 *
 * Visual C++, sigh. uses %I64d", rather than "%lld", so that is abstracted here
 */

#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    /* defined(__STLSOFT_COMPILER_IS_GCC) || */ \
    defined(__STLSOFT_COMPILER_IS_INTEL) || \
    defined(__STLSOFT_COMPILER_IS_MSVC)
# define FMT_SINT64_WIDTH_(x)           "%" #x "I64d"
#else
# define FMT_SINT64_WIDTH_(x)           "%" #x "lld"
#endif /* compiler */

/* /////////////////////////////////////////////////////////////////////////////
 * Constants
 */

#ifdef __STLSOFT_COMPILER_IS_INTEL
# pragma warning(disable : 1418)
#endif /* __STLSOFT_COMPILER_IS_INTEL */

/* Multi-part path/pattern delimiter */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1300
char const  PATH_SEPARATOR              =   ';';
#else /* ? compiler */
char const  PATH_SEPARATOR              =   filesystem_traits<char>::path_separator();
#endif /* compiler */

/* Path component delimiter */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1300
char const  PATH_NAME_SEPARATOR         =   '\\';
#else /* ? compiler */
char const  PATH_NAME_SEPARATOR         =   filesystem_traits<char>::path_name_separator();
#endif /* compiler */

/* Subdirectory wildcard pattern */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1300
char const  *PATTERN_ALL                =   "*.*";
#else /* ? compiler */
char const  *PATTERN_ALL                =   filesystem_traits<char>::pattern_all();
#endif /* compiler */

/* Illegal pattern characters string */

#if defined(_MSC_VER) /* For testing */ || \
    defined(WHEREIS_PLATFORM_IS_WIN32)
char const  *BAD_PATTERN_CHARS          =   ":\\";
#elif defined(WHEREIS_PLATFORM_IS_UNIX)
char const  *BAD_PATTERN_CHARS          =   "/";
#else
# error Unknown platform
#endif /* platform */

#ifdef __STLSOFT_COMPILER_IS_INTEL
# pragma warning(default : 1418)
#endif /* __STLSOFT_COMPILER_IS_INTEL */

/* /////////////////////////////////////////////////////////////////////////////
 * Function declarations
 *
 * (Primarily to keep Metrowerks happy, but that's a good thing to do!)
 */

static char *timestr(char *buffer, size_t cchBuffer, whereis_time_t const *ft); // Make Metrowerks happy

/* /////////////////////////////////////////////////////////////////////////////
 * Global variables
 */

static file_path_buffer	s_root;
static extension_map_t	s_extMap;

/* /////////////////////////////////////////////////////////////////////////////
 * Helper Functions
 */

char *timestr(char *buffer, size_t cchBuffer, whereis_time_t const *t)
{
#if defined(WHEREIS_PLATFORM_IS_WIN32)
    FILETIME    ftLocal;
    SYSTEMTIME  st;
    char        dateString[41];
    char        timeString[41];

    ::FileTimeToLocalFileTime(t, &ftLocal);
    ::FileTimeToSystemTime(&ftLocal, &st);

    ::GetDateFormat(LOCALE_USER_DEFAULT, 0, &st, NULL, dateString, stlsoft_num_elements(dateString));
    ::GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, NULL, timeString, stlsoft_num_elements(timeString));

#if 0
    // Digital Mars format (US based)
    sprintf(buffer, "%02d/%02d/%02d  %2d:%02d", st.wMonth, st.wDay, st.wYear % 100, st.wHour, st.wMinute);
#else /* ? 0 */
    // User-locale format
    sprintf(buffer, "%-10s  %-s", dateString, timeString);
#endif /* 0 */

    STLSOFT_SUPPRESS_UNUSED(cchBuffer);

#elif defined(WHEREIS_PLATFORM_IS_UNIX)

    struct tm   tm_ = *localtime(t);

    strftime(buffer, cchBuffer, "%c", &tm_);

#else
# error Unknown platform
#endif /* platform */

    return buffer;
}

static char *copy_entry_filename(char *dest, findfile_sequence::value_type const &entry)
{
#if defined(WHEREIS_PLATFORM_IS_WIN32)
	return strcpy(dest, entry.get_filename());
#elif defined(WHEREIS_PLATFORM_IS_UNIX)
	// Use the get_full_path_name()
	file_path_buffer	buffer;
	char				*pFile	=	NULL;
	size_t				cch		=	platform_stl::filesystem_traits<char>::get_full_path_name(entry, buffer.size(), &buffer[0], &pFile);

	STLSOFT_SUPPRESS_UNUSED(cch);

	return strcpy(dest, pFile);
#else
# error Unknown platform
#endif /* platform */
}

/* /////////////////////////////////////////////////////////////////////////////
 * Functionals
 */

// struct trace_file
//
// This functional traces individual file records to stdout.

struct trace_file
    : public unary_function<findfile_sequence::value_type const &, void>
{
	typedef	trace_file	class_type;

public:
    trace_file(char const *searchRoot, Trim::Trim trim, ss_uint_t flags)
        : m_flags(flags)
		, m_searchRoot(searchRoot)
		, m_trim(trim)
    {}

    void operator ()(findfile_sequence::value_type const &entry) const
    {
		file_path_buffer	path;

		switch(m_trim)
		{
			default:
				stlsoft_assert(0);
			case	Trim::full:
				strcpy(&path[0], c_str_ptr(entry));
				break;
			case	Trim::fileOnly:
				copy_entry_filename(&path[0], entry);
				break;
#if defined(_SYNSOFT_INTERNAL_BUILD) && \
	defined(WHEREIS_PLATFORM_IS_WIN32)
			case	Trim::fromSearchRoot:
				SynesisCrt::deriverelativepath(stlsoft_ns_qual(c_str_ptr)(entry), m_searchRoot, &path[0]);
				break;
			case	Trim::fromCurrentDirectory:
				SynesisCrt::deriverelativepath(stlsoft_ns_qual(c_str_ptr)(entry), stlsoft_ns_qual(c_str_ptr)(s_root), &path[0]);
				break;
#endif /* _SYNSOFT_INTERNAL_BUILD */
		}

        if(m_flags & Flags::verbose)
        {
            ss_sint64_t                             size;
            char                                    timebuf[50];
            char                                    atts[8 + 1];
#if defined(WHEREIS_PLATFORM_IS_WIN32)
            findfile_sequence::find_data_type const &findData   =   entry.get_find_data();
            DWORD const                             &att        =   findData.dwFileAttributes;
            int                                     i;

            for (i = 0; i < 8; ++i)
            {
                static char tab[] = "RHSVDAXX";

                if (att & (1 << i))
                    atts[i] = tab[i];
                else
                    atts[i] = '-';
            }
            atts[8] = 0;

            timestr(timebuf, stlsoft_num_elements(timebuf), &findData.ftLastWriteTime);
            size = findData.nFileSizeHigh * __STLSOFT_GEN_SINT64_SUFFIX(0x100000000) + findData.nFileSizeLow;
#elif defined(WHEREIS_PLATFORM_IS_UNIX)
# ifdef _MSC_VER // For testing
            struct _stat    st;

            _stat(entry, &st);
# else /* ? _MSC_VER */
            struct stat     st;

            stat(entry, &st);
# endif /* _MSC_VER */

            atts[0] = ((st.st_mode & S_IWRITE) == 0) ? 'R' : '-';   // R
            atts[1] = '-'; // H
            atts[2] = '-'; // S
            atts[3] = '-'; // V
            atts[4] = ((st.st_mode & S_IFMT) == S_IFDIR) ? 'D' : '-';   // D
            atts[5] = '-'; // A
            atts[6] = '-'; // X
            atts[7] = '-'; // X
            atts[8] = 0;

            size = st.st_size;
            timestr(timebuf, stlsoft_num_elements(timebuf), &st.st_mtime);
#else /* ? platform */
# error Unknown platform
#endif /* platform */
#ifdef _SYNSOFT_INTERNAL_BUILD
# if defined(WHEREIS_PLATFORM_IS_WIN32)
			if(m_flags & Flags::showVersionInfo)
			{
				SynesisWin::LPWinVerInfoA	verInfo	=	SynesisWin::WinVer_GetVersionInformationA(c_str_ptr(entry));
				char						version[101];

				if(NULL != verInfo)
				{
					sprintf(version, "%d.%d.%02d.%04d", verInfo->fileVerMajor, verInfo->fileVerMinor, verInfo->fileVerRevision, verInfo->fileVerBuild);

					SynesisWin::WinVer_CloseVersionInformationA(verInfo);
				}
				else
				{
					version[0] = '\0';
				}


				printf("%-23s  %16s %s  " FMT_SINT64_WIDTH_(9) "\t%s\n", timebuf, version, atts, size, path.c_str());
			}
			else
# endif /* WHEREIS_PLATFORM_IS_WIN32 */
#endif /* _SYNSOFT_INTERNAL_BUILD */
			{
				printf("%-23s  %s  " FMT_SINT64_WIDTH_(9) "\t%s\n", timebuf, atts, size, path.c_str());
			}
        }
        else
        {
            fprintf(stdout, "%s\n", path.c_str());
        }
    }

// Members
private:
	char const *const	m_searchRoot;
    Trim::Trim const	m_trim;
	ss_uint_t const		m_flags;

// Not to be implemented
private:
	class_type &operator =(class_type const &);
};

// struct process_searchspec
//
// This functional processes a search-spec within a given directory, passing any
// file to be processed by trace_file.

struct process_searchspec
    : public unary_function<tokeniser_string_t::value_type const &, void>
{
	typedef	process_searchspec	class_type;

public:
    process_searchspec(string const &path, char const *searchRoot, Trim::Trim trim, ss_uint_t flags)
        : m_path(path)
		, m_searchRoot(searchRoot)
		, m_trim(trim)
		, m_flags(flags)
    {}

    void operator ()(tokeniser_string_t::value_type const &value) const
    {
#ifdef WHEREIS_PLATFORM_IS_UNIX
		try
		{
#endif /* WHEREIS_PLATFORM_IS_UNIX */

			int					fs_flags	=	0;

			if(m_flags & Flags::showDirectories)
			{
				fs_flags |= findfile_sequence::directories;
			}
			if(m_flags & Flags::showFiles)
			{
				fs_flags |= findfile_sequence::files;
			}

			findfile_sequence   files(c_str_ptr(m_path), c_str_ptr(value), fs_flags);

			for_each(files.begin(), files.end(), trace_file(m_searchRoot, m_trim, m_flags));
#ifdef WHEREIS_PLATFORM_IS_UNIX
		}
		catch(unixstl::glob_sequence_exception &)
		{}
#endif /* WHEREIS_PLATFORM_IS_UNIX */
    }

// Members
private:
    string const		m_path;
	char const *const	m_searchRoot;
    Trim::Trim const	m_trim;
	ss_uint_t const		m_flags;

// Not to be implemented
private:
	class_type &operator =(class_type const &);
};

// struct process_path
//
// This functional creates a search sequence for each path, and then processes
// each one with the trace_file functional.

struct process_path
    : public unary_function<string const &, void>
{
	typedef	process_path	class_type;

public:
    process_path(string const &searchSpec, Trim::Trim trim, ss_uint_t flags)
        : m_searchSpec(searchSpec)
		, m_searchRoot(NULL)
		, m_trim(trim)
		, m_flags(flags)
    {}
    process_path(string const &searchSpec, char const *searchRoot, Trim::Trim trim, ss_uint_t flags)
        : m_searchSpec(searchSpec)
		, m_searchRoot(searchRoot)
		, m_trim(trim)
		, m_flags(flags)
    {}

    void operator ()(string const &dir) const
    {
		typedef	filesystem_traits<char>	traits_type;

		file_path_buffer	searchRoot_;
		char const			*searchRoot = (traits_type::get_full_path_name((NULL != m_searchRoot) ? m_searchRoot : stlsoft_ns_qual(c_str_ptr)(dir), searchRoot_.size(), &searchRoot_[0]), stlsoft_ns_qual(c_str_ptr)(searchRoot_));
        // Now split with the string_tokeniser into all the constituent paths
        tokeniser_string_t	specs(&m_searchSpec[0], PATH_SEPARATOR);

        for_each(specs.begin(), specs.end(), process_searchspec(dir, stlsoft_ns_qual(c_str_ptr)(searchRoot), m_trim, m_flags));
    }

// Members
private:
    string const		m_searchSpec;
	char const *const	m_searchRoot;
    Trim::Trim const	m_trim;
	ss_uint_t const		m_flags;

// Not to be implemented
private:
	class_type &operator =(class_type const &);
};

// struct trace_dir
//
// This functional provides processing of a directory, used by the recursive
// option. It process the current directory (via process_path), and then all
// subdirectories (via trace_dir) "calling" itself recursively.

struct trace_dir
    : public unary_function<string const &, void>
{
	typedef	trace_dir	class_type;

public:
    trace_dir(string const &searchSpec, Trim::Trim trim, ss_uint_t flags)
        : m_searchSpec(searchSpec)
		, m_searchRoot(NULL)
		, m_trim(trim)
		, m_flags(flags)
    {}
    trace_dir(string const &searchSpec, char const *searchRoot, Trim::Trim trim, ss_uint_t flags)
        : m_searchSpec(searchSpec)
		, m_searchRoot(searchRoot)
		, m_trim(trim)
		, m_flags(flags)
    {}

    void operator ()(char const *dir) const
    {
		typedef	filesystem_traits<char>	traits_type;

		file_path_buffer	searchRoot_;
		char const			*searchRoot = (traits_type::get_full_path_name((NULL != m_searchRoot) ? m_searchRoot : dir, searchRoot_.size(), &searchRoot_[0]), stlsoft_ns_qual(c_str_ptr)(searchRoot_));
        process_path		f(m_searchSpec, searchRoot, m_trim, m_flags);

        f(dir);

#ifdef WHEREIS_PLATFORM_IS_UNIX
		try
		{
#endif /* WHEREIS_PLATFORM_IS_UNIX */
			findfile_sequence   directories(stlsoft_ns_qual(c_str_ptr)(dir), PATTERN_ALL, findfile_sequence::directories);

			for_each(directories.begin(), directories.end(), trace_dir(m_searchSpec, searchRoot, m_trim, m_flags));
#ifdef WHEREIS_PLATFORM_IS_UNIX
		}
		catch(unixstl::glob_sequence_exception &)
		{}
#endif /* WHEREIS_PLATFORM_IS_UNIX */
    }

    template <ss_typename_param_k S>
    void operator ()(S const &dir) const
    {
        operator ()(stlsoft_ns_qual(c_str_ptr)(dir));
    }

private:
    string const		m_searchSpec;
	char const *const	m_searchRoot;
    Trim::Trim const	m_trim;
	ss_uint_t const		m_flags;

// Not to be implemented
private:
	class_type &operator =(class_type const &);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Function declarations
 */

static void usage(int bExit, char const *reason, int invalidArg, int argc, char *argv[]);
static char const *convert_path_separators(char const *path, char *buffer);
#ifdef WHEREIS_PLATFORM_IS_WIN32
static char const *get_driveroots(char *buffer);
#endif /* WHEREIS_PLATFORM_IS_WIN32 */

/* /////////////////////////////////////////////////////////////////////////////
 * Helper functionals
 */

struct is_empty_path
{
    template <ss_typename_param_k S>
    ss_bool_t operator ()(S const &s) const
    {
        return c_str_ptr(s)[0] == '\0';
    }
};

struct print_path
{
    template <ss_typename_param_k S>
    void operator ()(S const &s) const
    {
        fprintf(stdout, " %s\n", c_str_ptr(s));
    }
};

#ifdef WHEREIS_TRACE
struct trace_path
{
    template <ss_typename_param_k S>
    void operator ()(S const &s) const
    {
		size_t											len = c_str_len(s);
		auto_buffer<char, malloc_allocator<char>, 256>	buffer(3 + len + 1);

        sprintf(buffer, " %s\n", c_str_ptr(s));

        OutputDebugStringA(buffer);
    }
};
#endif /* WHEREIS_TRACE */

/* /////////////////////////////////////////////////////////////////////////////
 * main
 */

#ifdef WIN32
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long );
#endif /* WIN32 */

static int main_(int argc, char **argv)
{
	ss_bool_t   bDisplayTotal			=	false;
    ss_bool_t   bFromEnvironment		=   false;
    ss_bool_t   bSearchGivenRoots		=   false;
    ss_bool_t   bSearchCwd				=   false;
    ss_bool_t   bDumpSearchRoots		=   false;
    ss_bool_t   bVerbose				=   true;
    ss_bool_t   bRecursive				=   false;
    ss_bool_t   bSuppressRecursive		=   false;
	ss_bool_t   bShowVersionInfo		=   false;
	ss_bool_t   bSummariseExtensions	=   false;
	ss_bool_t	bShowDirectories		=	false;
	ss_bool_t	bShowFiles				=	false;
	ss_bool_t	bMarkDirs				=	false;
    Trim::Trim	trim					=   Trim::full;
    char const  *rootSpec				=   NULL;
    char const  *searchSpec		        =   NULL;
    char const  *envSpec			    =   NULL;
#ifdef WHEREIS_PLATFORM_IS_WIN32
    char        driveroots[105]			=   "";
#endif /* WHEREIS_PLATFORM_IS_WIN32 */
	int			totalFound				=	0;

    for(int i = 1; i < argc; ++i)
    {
        char const  *arg    =   argv[i];

        if(arg[0] == '-')
        {
			if(arg[1] == '-')
			{
				if(	0 == strcmp("--directories", arg) ||
					0 == strcmp("--dirs", arg))
				{
					bShowDirectories = true;
				}
				else if(0 == strcmp("--files", arg))
				{
					bShowFiles = true;
				}
#ifdef _SYNSOFT_INTERNAL_BUILD
				else if(0 == strcmp("--version", arg))
				{
					return show_version();
				}
#endif /* _SYNSOFT_INTERNAL_BUILD */
				else
				{
					usage(1, "Invalid argument(s) specified", i, argc, argv);
				}
            }
            else
            {
                switch(arg[1])
                {
                    case    '?':
                        usage(1, NULL, i, argc, argv);
                        break;
                    case    'd':
                        bDumpSearchRoots        =   true;
                        break;
                    case    'e':
                        bFromEnvironment        =   true;
                        envSpec                 =   arg + 2;
                        break;
                    case    'f':
                        trim                    =   Trim::fileOnly;
                        break;
#ifdef WHEREIS_PLATFORM_IS_WIN32
                    case    'h':
                        bSearchGivenRoots       =   true;
                        rootSpec                =   get_driveroots(driveroots);
                        break;
#endif /* WHEREIS_PLATFORM_IS_WIN32 */
                    case    'i':
                        bFromEnvironment        =   true;
                        envSpec                 =   "INCLUDE";
                        break;
                    case    'l':
                        bFromEnvironment        =   true;
                        envSpec                 =   "LIB";
                        break;
                    case    'm':
                        bMarkDirs				=   true;
                        break;
//                  case    'n':
//                      bDisplayTotal           =   true;
//                      break;
                    case    'r':
                        bSearchGivenRoots       =   true;
                        rootSpec = arg + 2;
                        break;
                    case    'p':
                        bFromEnvironment        =   true;
                        envSpec                 =   "PATH";
                        break;
                    case    's':
                        bVerbose                =   false;
                        break;
                    case    't':
                        trim                    =   Trim::fromCurrentDirectory;
                        break;
                    case    'u':
                        bRecursive              =   true;
                        break;
                    case    'v':
                        bVerbose                =   true;
                        break;
                    case    'w':
                        bSearchCwd              =   true;
                        break;
                    case    'x':
                        bSummariseExtensions    =   false;
                        break;
                    case    'R':
                        bSuppressRecursive      =   true;
                        break;
                    case    'F':
                        trim                    =   Trim::full;
                        break;
                    case    'T':
                        trim                    =   Trim::fromSearchRoot;
                        break;
#ifdef _SYNSOFT_INTERNAL_BUILD
                    case    'V':
                        bShowVersionInfo		=   true;
                        break;
#endif /* _SYNSOFT_INTERNAL_BUILD */
                    default:
//                          fprintf(stderr, "Unrecognised argument \"%s\"\n", arg);
                        usage(1, "Invalid argument(s) specified", i, argc, argv);
                        break;
                }
            }
        }
        else
        {
            if(rootSpec == NULL)
            {
                rootSpec = arg;
            }
            else if(searchSpec == NULL)
            {
                searchSpec = arg;
            }
            else
            {
                usage(1, "<root-paths> and <search-spec> already specified", i, argc, argv);
            }
        }
    }

	STLSOFT_SUPPRESS_UNUSED(bSummariseExtensions);

	if(	!bShowDirectories &&
		!bShowFiles)
	{
		bShowFiles = true;
	}

	if(	NULL == searchSpec &&
		NULL == rootSpec)
	{
        usage(1, "No search parameters specified", 0, argc, argv);
	}

    if( ((bSearchCwd != false) +
        (bFromEnvironment != false) +
        (bSearchGivenRoots != false)) > 1)
    {
        usage(1, "Cannot specify two or more of -w, -r, -e, -l, -i, -p", 0, argc, argv);
    }

    // Validate the args. This entails just moving the root-dir into the
    // search-spec if no root-dir was specified. This simplifies the
    // previous command-line processing.
    if(NULL == searchSpec)
    {
        if( NULL != rootSpec &&
            !bSearchGivenRoots)
        {
            searchSpec = rootSpec;
            rootSpec = NULL;
        }
        else
        {
            usage(1, "Unexpected condition. Please report to Synesis Software", 0, argc, argv);
        }
    }

    file_path_buffer	rootDir_;

    if(bFromEnvironment)
    {
        rootSpec = NULL;
    }
    else
    {
        // If no directory is specified, the current directory is assumed
        if( rootSpec == NULL ||
            *rootSpec == '\0')
        {
            strcpy(&rootDir_[0], basic_current_directory<char>());
            rootSpec = stlsoft_ns_qual(c_str_ptr)(rootDir_);
        }
    }

    if( bFromEnvironment &&
        (   envSpec == NULL ||
            *envSpec == '\0'))
    {
        usage(1, "Cannot search from environment when no environment variable specified", 0, argc, argv);
    }

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
_MSC_VER < 1200
	GetCurrentDirectory(s_root.size(), s_root);
#else /* ? compiler */
	platform_stl::filesystem_traits<char>::get_current_directory(s_root.size(), &s_root[0]);
#endif /* compiler */

    // Handle forward slashes
    string  _rootDir_(rootSpec != NULL ? rootSpec : "");
    string  _searchSpec_(searchSpec != NULL ? searchSpec : "");

    rootSpec    =   convert_path_separators(rootSpec, const_cast<char*>(_rootDir_.data()));
    searchSpec  =   convert_path_separators(searchSpec, const_cast<char*>(_searchSpec_.data()));

    // Handle illegal characters
    if(strpbrk(searchSpec, BAD_PATTERN_CHARS) != 0)
    {
        char    sz[101];

        sprintf(sz, "<search-spec> contains illegal characters: \"%s\"\n", searchSpec);
        usage(1, sz, 0, argc, argv);
    }

    searchpath_sorted_t     sorted;
	ss_uint_t				flags	=	0;

	if(bVerbose)
	{
		flags |= Flags::verbose;
	}

	if(bShowVersionInfo)
	{
		flags |= Flags::showVersionInfo;
	}

	if(bShowDirectories)
	{
		flags |= Flags::showDirectories;
	}

	if(bShowFiles)
	{
		flags |= Flags::showFiles;
	}

	if(bMarkDirs)
	{
		flags |= Flags::markDirectories;
	}

#ifdef WHEREIS_USING_WIN32_VERSION_INFO
	SynesisWin::WinVer_Init();
#endif /* WHEREIS_USING_WIN32_VERSION_INFO */

    // Commence the search
    if(bFromEnvironment)
    {
        basic_environment_variable<char>        ENVVAR(envSpec);
        tokeniser_string_t                      paths(ENVVAR, PATH_SEPARATOR);

#ifdef __STLSOFT_COMPILER_IS_DMC
        {
            tokeniser_string_t::const_iterator  begin   =   paths.begin();
            tokeniser_string_t::const_iterator  end     =   paths.end();

            for(; begin != end; ++begin)
            {
                sorted.push_back(*begin);
            }
        }
#else
        copy(paths.begin(), paths.end(), back_inserter(sorted));
#endif /* __STLSOFT_COMPILER_IS_DMC */

        // The roots may contain duplicate elements, so
        remove_duplicates_from_unordered_sequence(sorted, compare_path<char>());

        // They may also contain empty paths, so
        sorted.erase(remove_if(sorted.begin(), sorted.end(), is_empty_path()), sorted.end());

        if(bDumpSearchRoots)
        {
            fprintf(stdout, "Search roots:\n");
            for_each(sorted.begin(), sorted.end(), print_path());
        }

        // Now execute over the duplicate-free (but still in the correct search order)
        // sequence, applying the process_path functional to each one.
        if(bRecursive)
        {
            for_each(sorted.begin(), sorted.end(), trace_dir(searchSpec, trim, flags));
        }
        else
        {
            for_each(sorted.begin(), sorted.end(), process_path(searchSpec, trim, flags));
        }
    }
    else
    {
        tokeniser_string_t                      roots(rootSpec, PATH_SEPARATOR);

#ifdef __STLSOFT_COMPILER_IS_DMC
        {
            tokeniser_string_t::const_iterator  begin   =   roots.begin();
            tokeniser_string_t::const_iterator  end     =   roots.end();

            for(; begin != end; ++begin)
            {
                sorted.push_back(*begin);
            }
        }
#else
        copy(roots.begin(), roots.end(), back_inserter(sorted));
#endif /* __STLSOFT_COMPILER_IS_DMC */

        // The roots may contain duplicate elements, so
        remove_duplicates_from_unordered_sequence(sorted, compare_path<char>());

        // They may also contain empty paths, so
        sorted.erase(remove_if(sorted.begin(), sorted.end(), is_empty_path()), sorted.end());

        if(bDumpSearchRoots)
        {
            fprintf(stdout, "Search roots:\n");
            for_each(sorted.begin(), sorted.end(), print_path());
        }

        // Now execute over the duplicate-free (but still in the correct search order)
        // sequence, applying the trace_dir functional to each one.
        if(!bSuppressRecursive)
        {
            for_each(sorted.begin(), sorted.end(), trace_dir(searchSpec, trim, flags));
        }
        else
        {
            for_each(sorted.begin(), sorted.end(), process_path(searchSpec, trim, flags));
        }
    }

	if(bDisplayTotal)
	{
		printf("    %5d file(s) found\n", totalFound);
	}

#ifdef WHEREIS_USING_WIN32_VERSION_INFO
	SynesisWin::WinVer_Uninit();
#endif /* WHEREIS_USING_WIN32_VERSION_INFO */

    return 0;
}

int main(int argc, char *argv[])
{
	int	res;

#if 0
	::Sleep(100000);
#endif /* 0 */

	try
	{
		res = main_(argc, argv);
	}
	catch(...)
	{
		fprintf(stderr, "Fatal error: Uncaught exception in main_()\n");

		res = EXIT_FAILURE;
	}

	return res;
}

/* /////////////////////////////////////////////////////////////////////////////
 * usage
 */

static void usage(int bExit, char const *reason, int invalidArg, int argc, char *argv[])
{
    fprintf(stderr, "\n");
    fprintf(stderr, "  %s %s Tool, v%d.%d.%d.%04d\n", COMPANY_NAME, TOOL_NAME, _mccVerHi, _mccVerLo, _mccVerRev, _mccBldNum);
    fprintf(stderr, "\n");
    fprintf(stderr, "    incorporating Digital Mars technology\n");
    fprintf(stderr, "\n");

    if(NULL != reason)
    {
        fprintf(stderr, "  Error: %s\n", reason);
        fprintf(stderr, "\n");
    }

	if(0 < invalidArg)
	{
		fprintf(stderr, "  First invalid argument #%d: %s\n", invalidArg, argv[invalidArg]);
		fprintf(stderr, "  Arguments were (first bad marked *):\n\n");
		for(int i = 1; i < argc; ++i)
		{
			fprintf(stderr, "  %s%s\n", (i == invalidArg) ? "* " : "  ", argv[i]);
		}
        fprintf(stderr, "\n");
	}

    fprintf(stderr, "  USAGE 1: "
					"%s [{-w | -r<root-paths> | -p | -i | -l | -e<env-var> | -h}] [-u] [-d] "
					"[{<--dirs> | <--directories>}] | [<--files>] "
#if defined(_SYNSOFT_INTERNAL_BUILD) && \
	defined(WHEREIS_PLATFORM_IS_WIN32)
					"[{-v | -s}] [-f | -F | | -t | -T] [-V] "
#else /* ? _SYNSOFT_INTERNAL_BUILD */
					"[{-v | -s}] [-f | -F] "
#endif /* _SYNSOFT_INTERNAL_BUILD */
					"[<root-paths>] <search-spec>\n", EXE_NAME);
    fprintf(stderr, "    where:\n\n");
#if 0
    fprintf(stderr, "    -c             - case-sensitive search\n");
    fprintf(stderr, "    -C             - case-insensitive search; (default)\n");
#endif /* 0 */
    fprintf(stderr, "    -d             - displays the search root path(s)\n");
    fprintf(stderr, "    -e<env-var>    - searches in the directories specified in the environment variable <env-var>\n");
    fprintf(stderr, "    -f             - shows the filename and extension only\n");
    fprintf(stderr, "    -F             - shows the full path; (default) \n");
    fprintf(stderr, "    -h             - searches from the roots of all drives on the system\n");
    fprintf(stderr, "    -i             - searches in the directories specified in the INCLUDE environment variable\n");
    fprintf(stderr, "    -l             - searches in the directories specified in the LIB environment variable\n");
    fprintf(stderr, "    -m             - mark directories with a trailing path separator\n");
//    fprintf(stderr, "    -n             - Prints a total number of files found\n");
////    fprintf(stderr, "    -N             - Prints only the total number of files found\n");
#if defined(WHEREIS_PLATFORM_IS_WIN32)
    fprintf(stderr, "    -p             - searches in the Windows paths (the directories specified in the PATH environment variable)\n");
    fprintf(stderr, "    -r<root-paths> - searches from the given root path(s), separated by \';\', e.g.\n");
    fprintf(stderr, "                       -r\"c:\\windows;x:\\bin\"\n");
#elif defined(WHEREIS_PLATFORM_IS_UNIX)
    fprintf(stderr, "    -p             - searches in the system paths (the directories specified in the PATH environment variable)\n");
    fprintf(stderr, "    -r<root-paths> - searches from the given root path(s), separated by \':\', e.g.\n");
    fprintf(stderr, "                       -r\"/usr/include:/etc\"\n");
#else
# error Unknown platform
#endif /* platform */
    fprintf(stderr, "    -R             - suppresses recursive search\n");
    fprintf(stderr, "    -s             - succinct output. Prints path only\n");
#if defined(_SYNSOFT_INTERNAL_BUILD) && \
	defined(WHEREIS_PLATFORM_IS_WIN32)
    fprintf(stderr, "    -t             - trims path relative to the current directory\n");
    fprintf(stderr, "    -T             - trims path relative to the root directory(ies) specified for the search(es)\n");
#endif /* _SYNSOFT_INTERNAL_BUILD */
    fprintf(stderr, "    -u             - recursive search. (Default except for environment variable searches.)\n");
    fprintf(stderr, "    -v             - verbose output. Prints time, attributes, size and path; (default)\n");
#ifdef _SYNSOFT_INTERNAL_BUILD
    fprintf(stderr, "    -V             - displays the version information, if any, for the file. Suppressed by -s\n");
#endif /* _SYNSOFT_INTERNAL_BUILD */
    fprintf(stderr, "    -w             - searches from the current working directory\n");
    fprintf(stderr, "    -x             - summarises the file extensions\n");
    fprintf(stderr, "    --dirs         - search for directories\n");
    fprintf(stderr, "    --directories  - search for directories\n");
    fprintf(stderr, "    --files        - search for files; (default if --files and --dir(ectorie)s not specified)\n");
    fprintf(stderr, "    <search-spec>  - one or more file search specifications, separated by \';\',\n");
    fprintf(stderr, "                     eg.\n");
    fprintf(stderr, "                       \"*.exe\"\n");
    fprintf(stderr, "                       \"myfile.ext\"\n");
    fprintf(stderr, "                       \"*.exe;*.dll\"\n");
    fprintf(stderr, "                       \"*.xl?;report.*\"\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  USAGE 2: %s -?\n", EXE_NAME);
    fprintf(stderr, "\n");
    fprintf(stderr, "    Displays this help\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  Contact %s\n", _nameSynesisSoftware);
    fprintf(stderr, "    at \"%s\",\n", _wwwSynesisSoftware_SystemTools);
    fprintf(stderr, "    or, via email, at \"%s\"\n", _emailSynesisSoftware_SystemTools);
    fprintf(stderr, "\n");
    fprintf(stderr, "  Contact Digital Mars\n");
    fprintf(stderr, "    at \"www.digitalmars.com\",\n");
    fprintf(stderr, "    or, via email, at \"software@digitalmars.com\"\n");
//    fprintf(stderr, "\n");

    if(bExit)
    {
        exit(EXIT_FAILURE);
    }
}

char const *convert_path_separators(char const *path, char *buffer)
{
#if defined(WHEREIS_PLATFORM_IS_WIN32)
    char const  chWrong =   '/';
#elif defined(WHEREIS_PLATFORM_IS_UNIX)
    char const  chWrong =   '\\';
#else
# error Unknown platform
#endif /* platform */

    if( path != 0 &&
        0 != strchr(path, chWrong))
    {
        char    *pch;

        path = strcpy(buffer, path);

        for(pch = buffer; 0 != (pch = strchr(pch + 1, chWrong)); )
        {
            *pch = PATH_NAME_SEPARATOR;
        }
    }

    return path;
}

#ifdef WHEREIS_PLATFORM_IS_WIN32
char const *get_driveroots(char *const buffer)
{
    char    drv[4] = "?:\\";

    *buffer = '\0';

    for(char ch = 'a'; ch <= 'z'; ++ch)
    {
        drv[0] = ch;

        long    type    =   GetDriveTypeA(drv);

        if(DRIVE_FIXED == type)
        {
            strcat(buffer, drv);
            strcat(buffer, ";");
        }
    }

    return buffer;
}
#endif /* WHEREIS_PLATFORM_IS_WIN32 */

/* ////////////////////////////////////////////////////////////////////////// */
