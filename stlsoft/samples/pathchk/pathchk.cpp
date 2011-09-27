/* /////////////////////////////////////////////////////////////////////////////
 * File:        pathchk.cpp
 *
 * Purpose:     Implementation file for the pathchk project.
 *
 * Created:     28th April 2003
 * Updated:     30th May 2004
 *
 * Status:      Wizard-generated
 *
 * License:     (Licensed under the Synesis Software Open License)
 *
 *              Copyright (C) 1999-2003, Synesis Software Pty Ltd.
 *              All rights reserved.
 *
 *              www:        http://www.synesis.com.au/software
 *
 *              email:      software@synesis.com.au
 *
 *              This source code is placed into the public domain 2003
 *              by Synesis Software Pty Ltd. There are no restrictions
 *              whatsoever to your use of the software.
 *
 *              This source code is provided by Synesis Software Pty Ltd "as is"
 *              and any warranties, whether expressed or implied, including, but
 *              not limited to, the implied warranties of merchantability and
 *              fitness for a particular purpose are disclaimed. In no event
 *              shall the Synesis Software Pty Ltd be liable for any direct,
 *              indirect, incidental, special, exemplary, or consequential
 *              damages (including, but not limited to, procurement of
 *              substitute goods or services; loss of use, data, or profits; or
 *              business interruption) however caused and on any theory of
 *              liability, whether in contract, strict liability, or tort
 *              (including negligence or otherwise) arising in any way out of
 *              the use of this software, even if advised of the possibility of
 *              such damage.
 *
 *              Neither the name of Synesis Software Pty Ltd nor the names of
 *              any subdivisions, employees or agents of Synesis Software Pty
 *              Ltd, nor the names of any other contributors to this software
 *              may be used to endorse or promote products derived from this
 *              software without specific prior written permission.
 *
 * ////////////////////////////////////////////////////////////////////////// */

#include <windows.h>
#include <stdio.h>

#include <stlsoft.h>

#include <stlsoft_string_tokeniser.h>

stlsoft_ns_using(string_tokeniser)
stlsoft_ns_using(string_tokeniser_ignore_blanks)
stlsoft_ns_using(string_tokeniser_type_traits)
stlsoft_ns_using(string_tokeniser_comparator)

#include <stlsoft_static_string.h>

stlsoft_ns_using(basic_static_string)

#include <stlsoft_simple_string.h>

stlsoft_ns_using(basic_simple_string)

#include <winstl.h>
#include <winstl_environment_variable.h>

winstl_ns_using(basic_environment_variable)

#include <stlsoft_string_access.h>
stlsoft_ns_using(c_str_ptr)

#include <algorithm>

stlsoft_ns_using_std(for_each)

/* ////////////////////////////////////////////////////////////////////////// */

#ifdef _SYNSOFT_INTERNAL_BUILD
# include <MWFlAttr.h>					// FILE_ATTRIBUTE_ERROR
# include <MLCompId.h>					// The _xxxSynesisSoftware constants
#else /* ? _SYNSOFT_INTERNAL_BUILD */
 #ifndef FILE_ATTRIBUTE_ERROR
 # define FILE_ATTRIBUTE_ERROR			0xffffffff
 #endif /* FILE_ATTRIBUTE_ERROR */
# define _nameSynesisSoftware				"Synesis Software (Pty) Ltd"
# define _wwwSynesisSoftware_SystemTools	"http://synesis.com.au/"
# define _emailSynesisSoftware_SystemTools	"software@synesis.com.au"
#endif /* _SYNSOFT_INTERNAL_BUILD */

/* /////////////////////////////////////////////////////////////////////////////
 * Warnings
 */

#ifdef __STLSOFT_COMPILER_IS_INTEL
# pragma warning(disable : 981)
#endif /* __STLSOFT_COMPILER_IS_INTEL */

/* /////////////////////////////////////////////////////////////////////////////
 * Helper functions
 */

static void usage(int bExit, char const *reason);

typedef basic_simple_string<char>					string_t;
typedef basic_static_string<char, _MAX_PATH + 1>	static_string_t;
#if 0
typedef	string_tokeniser<string_t, char>			tokeniser_t;
#else
typedef	string_tokeniser<	string_t
						,	char
						,	string_tokeniser_ignore_blanks<true>
						,	static_string_t
						,	string_tokeniser_type_traits<string_t, static_string_t>
						,	string_tokeniser_comparator<	char
														,	string_t
														,	string_tokeniser_type_traits<string_t, static_string_t>
														>
						>							tokeniser_t;
#endif /* 0 */
typedef	basic_environment_variable<char>			envvar_t;

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

extern "C" const int	_mccVerHi	=	__SYV_MAJOR;
extern "C" const int	_mccVerLo	=	__SYV_MINOR;
extern "C" const int	_mccVerRev	=	__SYV_REVISION;
extern "C" const int	_mccBldNum	=	__SYV_BUILDNUMBER;

#else /* ? _SYNSOFT_INTERNAL_BUILD */
extern "C" const int	_mccVerHi	=	1;
extern "C" const int	_mccVerLo	=	1;
extern "C" const int	_mccVerRev	=	1;
extern "C" const int	_mccBldNum	=	3;
#endif /* _SYNSOFT_INTERNAL_BUILD */

#define	COMPANY_NAME	"Synesis Software"
#define	TOOL_NAME		"Operating System Version Detection"
#define	EXE_NAME		"osver"

/* /////////////////////////////////////////////////////////////////////////////
 * Constants and definitions
 */

const char	DELIMITER	=	';';

/* ////////////////////////////////////////////////////////////////////////// */

struct validate_file
{
	template<ss_typename_param_k S>
	void operator ()(S const &s)
	{
		DWORD	dwAttr	=	::GetFileAttributes(c_str_ptr(s));

		if(dwAttr == 0xFFFFFFFF)
		{
			fprintf(stdout, "\"%s\" does not exist\n", c_str_ptr(s));
		}
	}

	/* VC5 doesn't like basic_static_string's CCH parameter, so we need to provide
	 * an explicit operator() here
	 */
	void operator ()(static_string_t const &s)
	{
		operator ()(s.c_str());
	}
};

/* ////////////////////////////////////////////////////////////////////////// */

int main(int argc, char **argv)
{
#ifdef _DEBUG
	puts("pathchk: " __STLSOFT_COMPILER_LABEL_STRING);
#endif /* _DEBUG */

	if(argc > 1)
	{
		char const	*arg	=	argv[1];

		if(0 == strcmp("-?", argv[1]) ||
#ifdef SYSTOOLS_ALLOW_SLASH_OPTIONS
				0 == strcmp("/?", argv[1]) ||
#endif /* SYSTOOLS_ALLOW_SLASH_OPTIONS */
				0)
		{
			usage(1, NULL);
		}
#ifdef _SYNSOFT_INTERNAL_BUILD
		else if(0 == strcmp("--version", arg))
		{
			return show_version();
		}
#endif /* _SYNSOFT_INTERNAL_BUILD */
		else
		{
			usage(1, "Unrecognised argument");
		}
	}
	else
	{
		// Retrive and tokenise the PATH environment variable ...
		envvar_t	PATHS("PATH");
		tokeniser_t	tokens(PATHS, DELIMITER);

		// ... and then print out any that do not exist
		for_each(tokens.begin(), tokens.end(), validate_file());
	}

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */

void usage(int bExit, char const *reason)
{
    fprintf(stderr, "\n");
	fprintf(stderr, "  %s %s Tool, v%d.%d.%d.%04d\n", COMPANY_NAME, TOOL_NAME, _mccVerHi, _mccVerLo, _mccVerRev, _mccBldNum);
    fprintf(stderr, "\n");

	if(NULL != reason)
	{
		fprintf(stderr, "  Error: %s\n", reason);
		fprintf(stderr, "\n");
	}

	fprintf(stderr, "  USAGE 1: %\n", EXE_NAME);
    fprintf(stderr, "\n");
	fprintf(stderr, "    Lists all invalid components of the system PATH\n");
	fprintf(stderr, "    environment variable\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  USAGE 2: %s -?\n", EXE_NAME);
    fprintf(stderr, "\n");
    fprintf(stderr, "    Displays this help\n");
    fprintf(stderr, "\n");
	fprintf(stderr, " Contact %s\n", _nameSynesisSoftware);
	fprintf(stderr, "   at \"%s\",\n", _wwwSynesisSoftware_SystemTools);
	fprintf(stderr, "   or via email at \"%s\"\n", _emailSynesisSoftware_SystemTools);
	fprintf(stderr, "\n");

	if(bExit)
	{
		exit(EXIT_FAILURE);
	}
}

/* ////////////////////////////////////////////////////////////////////////// */
