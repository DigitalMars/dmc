/* /////////////////////////////////////////////////////////////////////////////
 * File:        hardlink.cpp
 *
 * Purpose:     Creates hard-links on Windows 2000/XP machines.
 *
 * Created:     29th September 2001
 * Updated:     30th May 2004
 *
 * Status:      Wizard-generated
 *
 * License:     (Licensed under the Synesis Software Open License)
 *
 *              Copyright (C) 1999-2003, Synesis Software Pty Ltd.
 *              All rights reserved.
 *
 *              www:        http://www.synesis.com.au
 *                          http://www.synesis.com.au/software
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


// This is defined to illustrate the utility of having the headers compile in a
// "verbose" fashion. When you get sick of them, just comment it out.
#define	_STLSOFT_COMPILE_VERBOSE

////////////////////////////////////////////////////////////////////////////////
// Includes

// NOTE: the symbol _SYNSOFT_INTERNAL_BUILD is used when I'm building this for
// the Synesis Software system tools (http://synesis.com.au/r_systools.html, if
// you're interested). You can just ignore it

#include <stdio.h>

#ifdef _SYNSOFT_INTERNAL_BUILD
# include <MWBase.h>
# include <MRStdio.h>
# include <MRStdLib.h>
#endif /* _SYNSOFT_INTERNAL_BUILD */

#include <stlsoft.h>					// STLSoft root-header

stlsoft_ns_using(ss_bool_t)

#include <winstl_filesystem_traits.h>	// winstl::filesystem_traits

winstl_ns_using(filesystem_traits)

#include <winstl_findfile_sequence.h>	// winstl::findfile_sequence

winstl_ns_using(findfile_sequence_a)

#include <winstl_system_version.h>		// winstl::system_version

winstl_ns_using(system_version)

#include <winstl_functions.h>

winstl_ns_using(FormatMessage)

/* ////////////////////////////////////////////////////////////////////////// */

#ifdef _SYNSOFT_INTERNAL_BUILD
# include <MWFlAttr.h>					// FILE_ATTRIBUTE_ERROR
# include <MLCompId.h>					// The _xxxSynesisSoftware constants
#else /* ? _SYNSOFT_INTERNAL_BUILD */
# ifndef FILE_ATTRIBUTE_ERROR
#  define FILE_ATTRIBUTE_ERROR			0xffffffff
# endif /* FILE_ATTRIBUTE_ERROR */
# define _nameSynesisSoftware				"Synesis Software (Pty) Ltd"
# define _wwwSynesisSoftware_SystemTools	"http://synesis.com.au/"
# define _emailSynesisSoftware_SystemTools	"software@synesis.com.au"
#endif /* _SYNSOFT_INTERNAL_BUILD */

/* ////////////////////////////////////////////////////////////////////////// */

#ifdef _SYNSOFT_INTERNAL_BUILD
# include "MBldHdr.h"
# include "showver.h"
#else /* ? _SYNSOFT_INTERNAL_BUILD */

#undef __SYV_MAJOR
#define __SYV_MAJOR 1

#undef __SYV_MINOR
#define __SYV_MINOR 2

#undef __SYV_REVISION
#define __SYV_REVISION 4

#undef __SYV_BUILDNUMBER
#define __SYV_BUILDNUMBER 8

#undef _MBN_VER_NUM
#define _MBN_VER_NUM "0007"

#endif /* _SYNSOFT_INTERNAL_BUILD */

////////////////////////////////////////////////////////////////////////////////
// Version information constants

extern "C" const int	_mccVerHi	=	__SYV_MAJOR;
extern "C" const int	_mccVerLo	=	__SYV_MINOR;
extern "C" const int	_mccVerRev	=	__SYV_REVISION;
extern "C" const int	_mccBldNum	=	__SYV_BUILDNUMBER;

#define	COMPANY_NAME	"Synesis Software"
#define	TOOL_NAME		"Hard-link Creation"
#define	EXE_NAME		"hardlink"

////////////////////////////////////////////////////////////////////////////////
// Typedefs

typedef	BOOL (WINAPI *PFnCreateHardLink)(LPCTSTR , LPCTSTR , LPSECURITY_ATTRIBUTES );

////////////////////////////////////////////////////////////////////////////////
// Statics

// Borland defines _WIN32_WINNT as >= 0x0500, so CreateHardLinkA is visible. In
// this case we redefine CreateHardLinkA here so when we use it in main() we'll
// by using our GetProcAddress()'ed one.
#ifdef __BORLANDC__
# define CreateHardLinkA	__CreateHardLinkA
#endif /* __BORLANDC__ */

static HINSTANCE			s_hinst			=	::LoadLibrary("Kernel32");
static PFnCreateHardLink	CreateHardLinkA	=	(PFnCreateHardLink)::GetProcAddress(s_hinst, "CreateHardLinkA");

////////////////////////////////////////////////////////////////////////////////
// Helper functions

static void usage(int bExit, char const *reason);
ss_bool_t hardlink(const char *srcName, const char *linkName);

////////////////////////////////////////////////////////////////////////////////
// main

int main(int argc, char **argv)
{
	if( !system_version::winnt() ||
		system_version::major() < 5)
	{
		fprintf(stderr, "%s does not operate on operating systems earlier than Windows 2000\n", EXE_NAME);
	}
	else if(argc < 2)
	{
		usage(1, NULL);
	}
	else if(argc > 3)
	{
		usage(1, "Invalid argument(s) specified");
	}
	else
	{
		if(0 == strcmp("-?", argv[1]))
		{
			usage(1, NULL);
		}
#ifdef _SYNSOFT_INTERNAL_BUILD
		else if(0 == strcmp("--version", argv[1]))
		{
			return show_version();
		}
#endif /* _SYNSOFT_INTERNAL_BUILD */

		const char	*srcName	=	argv[1];
		const char	*linkName	=	(argc > 2) ? argv[2] : ".";

		if(strpbrk(srcName, "*?") != NULL)
		{
			findfile_sequence_a					files(srcName, findfile_sequence_a::files);
			findfile_sequence_a::const_iterator	begin	=	files.begin();
			findfile_sequence_a::const_iterator	end		=	files.end();

			char	szDrive[_MAX_DRIVE];
			char	szDir[_MAX_DIR];

			_splitpath(srcName, szDrive, szDir, NULL, NULL);

			for(; begin != end; ++begin)
			{
				//
				char	szFName[_MAX_FNAME];
				char	szExt[_MAX_EXT];
				char	szFileName[_MAX_PATH];

				_splitpath(*begin, NULL, NULL, szFName, szExt);
				_makepath(szFileName, szDrive, szDir, szFName, szExt);

				fprintf(stdout, "%s => %s ", szFileName, linkName);
				if(	hardlink(szFileName, linkName) ||
					stdout != stderr)
				{
					fprintf(stdout, "\n");
				}
			}
		}
		else
		{
			if(!hardlink(srcName, linkName))
			{
				exit(false);
			}
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Helper functions

ss_bool_t hardlink(const char *srcName, const char *linkName)
{
	ss_bool_t	bRet		=	false;
	char		linkPath[_MAX_PATH];
Assess:
	DWORD		srcAttr		=	::GetFileAttributes(srcName);
	DWORD		linkAttr	=	::GetFileAttributes(linkName);

	if(	(	srcName[0] == '-' ||
			srcName[0] == '/') &&
		(	srcName[1] == '?' ||
			(	(	srcName[1] == 'h' ||
					srcName[1] == 'H') &&
				srcName[2] == '\0')))
	{
	}
	else if(srcAttr == FILE_ATTRIBUTE_ERROR)
	{
		fprintf(stderr, "Source file \"%s\" does not exist\n", srcName);
	}
	else if(srcAttr & FILE_ATTRIBUTE_DIRECTORY)
	{
		fprintf(stderr, "Source file \"%s\" specifies a directory\n", srcName);
	}
	else if(linkAttr != FILE_ATTRIBUTE_ERROR &&
			(linkAttr & FILE_ATTRIBUTE_DIRECTORY))
	{
		typedef	filesystem_traits<char>	fst_a_t;

		char	szDrive[_MAX_DRIVE];
		char	szDir[_MAX_DIR];
		char	szFName[_MAX_FNAME];
		char	szExt[_MAX_EXT];

		_splitpath(srcName, NULL, NULL, szFName, szExt);
		strcpy(linkPath, linkName);
		fst_a_t::ensure_dir_end(linkPath);
		linkName = linkPath;
		_splitpath(linkName, szDrive, szDir, NULL, NULL);
		_makepath(linkPath, szDrive, szDir, szFName, szExt);

		linkName = linkPath;

		goto Assess;	// Scary!!
	}
	else if(linkAttr != FILE_ATTRIBUTE_ERROR)
	{
		fprintf(stderr, "Destination file \"%s\" already exists", linkName);
	}
	else
	{
		::SetLastError(ERROR_SUCCESS);

		// Create the link ...
		if(NULL == CreateHardLinkA)
		{
			fprintf(stderr, "The CreateHardLinkA() function was not found in KERNEL32.DLL\n!");
		}
		else if(!CreateHardLinkA(linkName, srcName, NULL))
		{
			// ... or trace why it could not be created.
			DWORD	dwErr	=	::GetLastError();
			char	szMsg[512];

			FormatMessage(dwErr, szMsg, stlsoft_num_elements(szMsg));

			fprintf(stderr, "The link creation failed. Error: %d, %s", dwErr, szMsg);
		}
		else
		{
			bRet = true;
		}
	}

	return bRet;
}

/* ////////////////////////////////////////////////////////////////////////// */

static void usage(int bExit, char const *reason)
{
    fprintf(stderr, "\n");
	fprintf(stderr, "  %s %s Tool, v%d.%d.%d.%04d\n", COMPANY_NAME, TOOL_NAME, _mccVerHi, _mccVerLo, _mccVerRev, _mccBldNum);
    fprintf(stderr, "\n");

	if(NULL != reason)
	{
		fprintf(stderr, "  Error: %s\n", reason);
		fprintf(stderr, "\n");
	}

	fprintf(stderr, "  USAGE 1: %s <src-spec> [<link-dest>]\n", EXE_NAME);
	fprintf(stderr, "\n");
	fprintf(stderr, "    <src-spec>  -   the source, either a single file, or wildcards\n");
	fprintf(stderr, "    <link-dest> -   destination file, or directory. Current\n");
	fprintf(stderr, "                    directory assumed if not specified\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    Creates hard link(s) from the <src-spec> to the <link-dest>\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  USAGE 2: %s -?\n", EXE_NAME);
    fprintf(stderr, "\n");
    fprintf(stderr, "    Displays this help\n");
    fprintf(stderr, "\n");
	fprintf(stderr, "  Contact %s\n", _nameSynesisSoftware);
	fprintf(stderr, "    at \"%s\",\n", _wwwSynesisSoftware_SystemTools);
	fprintf(stderr, "    or, via email, at \"%s\"\n", _emailSynesisSoftware_SystemTools);
    fprintf(stderr, "\n");

    if(bExit)
    {
        exit(EXIT_FAILURE);
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
