/* /////////////////////////////////////////////////////////////////////////////
 * File:        rdempty.cpp
 *
 * Purpose:     Implementation file for the rdempty project.
 *
 * Created:     21st August 2003
 * Updated:     30th June 2004
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

#include <MLBase.h>
#include <stdio.h>

#include <stlsoft.h>

#if __SYNSOFT_DVS_OS == __SYNSOFT_VAL_OS_UNIX

# include <unixstl.h>

namespace platform_stl	=	unixstl;

# include <unixstl_findfile_sequence.h>

 unixstl_ns_using(findfile_sequence)

# include <unixstl_file_path_buffer.h>

 typedef unixstl_ns_qual(file_path_buffer_a)		file_path_buffer;

# include <unistd.h>

#elif __SYNSOFT_DVS_OS == __SYNSOFT_VAL_OS_WIN

# include <winstl.h>

namespace platform_stl	=	winstl;

# include <winstl_file_path_buffer.h>

 typedef winstl_ns_qual(file_path_buffer_a)			file_path_buffer;

# include <winstl_findfile_sequence.h>

 typedef winstl_ns_qual(findfile_sequence_a)		findfile_sequence;

# include <direct.h>

#else /* ? __SYNSOFT_DVS_OS */
# error Operating system not discriminated
#endif /* __SYNSOFT_DVS_OS */

/* ////////////////////////////////////////////////////////////////////////// */

#ifdef _SYNSOFT_INTERNAL_BUILD
# include <MLCompId.h>					// The _xxxSynesisSoftware constants
#else /* ? _SYNSOFT_INTERNAL_BUILD */
# define _nameSynesisSoftware				"Synesis Software (Pty) Ltd"
# define _wwwSynesisSoftware_SystemTools	"http://synesis.com.au/"
# define _emailSynesisSoftware_SystemTools	"software@synesis.com.au"
#endif /* _SYNSOFT_INTERNAL_BUILD */

/* /////////////////////////////////////////////////////////////////////////////
 * Version information
 *
 * Normally, Synesis Software components & tools are "versioned" by making use
 * of tool-generated header files, one of which - MBldHdr.h - defines symbolic
 * constants representing major, minor, revision and build numbers.
 *
 * Since this file is to be available in a more public forum, including on the
 * Digital Mars site, these constants are merely defined here, rather than
 * encumber any developers who may wish to compile it with a lot of
 * Synesis-specific gunk.
 */

#ifdef _SYNSOFT_INTERNAL_BUILD
# include "MBldHdr.h"
# include "showver.h"

 const int	_mccVerHi	=	__SYV_MAJOR;
 const int	_mccVerLo	=	__SYV_MINOR;
 const int	_mccVerRev	=	__SYV_REVISION;
 const int	_mccBldNum	=	__SYV_BUILDNUMBER;

#else /* ? _SYNSOFT_INTERNAL_BUILD */
 const int	_mccVerHi	=	1;
 const int	_mccVerLo	=	0;
 const int	_mccVerRev	=	5;
 const int	_mccBldNum	=	6;
#endif /* _SYNSOFT_INTERNAL_BUILD */

#define	COMPANY_NAME	"Synesis Software"
#define	TOOL_NAME		"Empty Directory Removal"
#define	EXE_NAME		"rdempty"

/* /////////////////////////////////////////////////////////////////////////////
 * Function declarations
 */

static void usage(int bExit, char const *reason);
static bool directory_is_empty(char const *dir);
static int empty_directory(char const *dir, int bVerbose);

/* /////////////////////////////////////////////////////////////////////////////
 * Constants
 */

#if __SYNSOFT_DVS_OS == __SYNSOFT_VAL_OS_UNIX || \
    __SYNSOFT_DVS_OS == __SYNSOFT_VAL_OS_WIN
#define	WILDCARDS_ALL		platform_stl::filesystem_traits<char>::pattern_all()
#else /* ? __SYNSOFT_DVS_OS */
# error Operating system not discriminated
#endif /* __SYNSOFT_DVS_OS */

/* ////////////////////////////////////////////////////////////////////////// */

int main(int argc, char *argv[])
{
    int					iRet		=   0;
    int					i;
	char const			*root		=	NULL;
	file_path_buffer	root_;
	char				*p;
	int					bVerbose	=	0;

#ifdef _DEBUG
	puts("rdempty: " __STLSOFT_COMPILER_LABEL_STRING);
#endif /* _DEBUG */

	for(i = 1; i < argc; ++i)
	{
		const char	*arg	=	argv[i];

		if(arg[0] == '-')
		{
			if(0 == strcmp("-?", arg))
			{
				usage(1, NULL);
			}
#ifdef _SYNSOFT_INTERNAL_BUILD
			else if(0 == strcmp("--version", arg))
			{
				return show_version();
			}
#endif /* _SYNSOFT_INTERNAL_BUILD */
			else if(0 == strcmp("-v", arg))
			{
				bVerbose = 1;
			}
			else
			{
				usage(1, "Invalid argument(s) specified");
			}
		}
		else
		{
			/* other arguments */
			if(NULL == root)
			{
				root = arg;
			}
			else
			{
				usage(1, "Invalid argument(s) specified");
			}
		}
	}

	/* . */
	if(NULL == root)
	{
		root = ".";
	}

	if(!platform_stl::filesystem_traits<char>::get_full_path_name(root, root_.size(), &root_[0], &p))
	{
		printf("Directory \"%s\" does not exist!\n", root);
	}
	else
	{
		root = stlsoft_ns_qual(c_str_ptr)(root_);

		if(bVerbose)
		{
			printf("Cleaning directory \"%s\"\n", root);
		}

		int	cRemoved	=	empty_directory(root, bVerbose);

		if(bVerbose)
		{
			printf("Removed %d empty directories\n", cRemoved);
		}
	}

    return iRet;
}

/* ////////////////////////////////////////////////////////////////////////// */

bool directory_is_empty(char const *dir)
{
	return findfile_sequence(dir, WILDCARDS_ALL, findfile_sequence::files | findfile_sequence:: directories).empty();
}

int empty_directory(char const *dir, int bVerbose)
{
	int	cRemoved	=	0;

	try
	{
		findfile_sequence					dirs(dir, WILDCARDS_ALL, findfile_sequence::directories);
		findfile_sequence::const_iterator	begin = dirs.begin();
		findfile_sequence::const_iterator	end		= dirs.end();

		for(; begin != end; ++begin)
		{
			cRemoved += empty_directory(stlsoft_ns_qual(c_str_ptr)(*begin), bVerbose);
		}

		if(directory_is_empty(dir))
		{
			if(0 == rmdir(dir))
			{
				++cRemoved;

				if(bVerbose)
				{
					printf("Removed \"%s\"\n", dir);
				}
			}
			else
			{
				printf("Could not remove \"%s\"\n", dir);
			}
		}
	}
	catch(std::exception &x)
	{
		fprintf(stderr, "Error occured when processing directory: %s; %s\n", dir, x.what());
	}

	return cRemoved;
}

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

    fprintf(stderr, "  USAGE 1: %s [-v] [<dir>]\n", EXE_NAME);
    fprintf(stderr, "    where:\n\n");
    fprintf(stderr, "    <dir>          - root directory in search (defaults to current directory)\n");
    fprintf(stderr, "    -v             - prints out names of deleted directories, and total deleted\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    Removes all empty sub-directories of the given directory\n");
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
