/* /////////////////////////////////////////////////////////////////////////////
 * File:        lnunique.cpp
 *
 * Purpose:     Implementation file for the lnunique project.
 *
 * Created:     2nd July 2003
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

#include <stdio.h>

#include <stlsoft.h>
#include <stlsoft_simple_string.h>

#ifndef __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT
stlsoft_ns_using(c_str_ptr)
#endif /* __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT */

#ifdef _SYNSOFT_INTERNAL_BUILD
# include "showver.h"

# include <MLCompId.h>
# include "MBldHdr.h"
#endif /* _SYNSOFT_INTERNAL_BUILD */

#include <map>

/* ////////////////////////////////////////////////////////////////////////// */

#ifdef _SYNSOFT_INTERNAL_BUILD
# include <MLCompId.h>					// The _xxxSynesisSoftware constants
#else /* ? _SYNSOFT_INTERNAL_BUILD */
# define _nameSynesisSoftware				"Synesis Software (Pty) Ltd"
# define _wwwSynesisSoftware_SystemTools	"http://synesis.com.au/systools.html"
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
#define __SYV_REVISION 3

#undef __SYV_BUILDNUMBER
#define __SYV_BUILDNUMBER 7

#undef _MBN_VER_NUM
#define _MBN_VER_NUM "0007"

#endif /* _SYNSOFT_INTERNAL_BUILD */

/* ////////////////////////////////////////////////////////////////////////// */

const int	_mccVerHi	=	__SYV_MAJOR;
const int	_mccVerLo	=	__SYV_MINOR;
const int	_mccVerRev	=	__SYV_REVISION;
const int	_mccBldNum	=	__SYV_BUILDNUMBER;

#define	COMPANY_NAME	"Synesis Software"
#define	TOOL_NAME		"Duplicate Line Trimming"
#define	EXE_NAME		"lnunique"

/* ////////////////////////////////////////////////////////////////////////// */

typedef stlsoft_ns_qual(basic_simple_string)<char>	string_t;
typedef stlsoft_ns_qual_std(map)<string_t, int>		map_t;

/* ////////////////////////////////////////////////////////////////////////// */

static void usage(int bExit, char const *reason);

/* ////////////////////////////////////////////////////////////////////////// */

int main(int argc, char *argv[])
{
    int			iRet    =   0;
	bool		bShowCount	=	false;
	char const	*input		=	NULL;
	char const	*output		=	NULL;

	for(int i = 1; i < argc; ++i)
	{
		char const	*arg	=	argv[i];

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
		else if(arg[0] == '-')
		{
			switch(arg[1])
			{
				case	'c':
					bShowCount	=	true;
					break;
				default:
					usage(1, "unrecognised argument");
					break;
			}
		}
		else
		{
			if(input == NULL)
			{
				input = arg;
			}
			else if(output == NULL)
			{
				output = arg;
			}
			else
			{
				usage(1, "unrecognised argument");
			}
		}
	}

	FILE	*in		=	(NULL != input) ? fopen(input, "r") : stdin;
	FILE	*out	=	(NULL != output) ? fopen(output, "w") : stdout;

	if(NULL == in)
	{
		fprintf(stderr, "Could not open \"%s\"!\n", argv[1]);

		iRet = 1;
	}
	else if(NULL == out)
	{
		fprintf(stderr, "Could not create \"%s\"!\n", argv[2]);

		iRet = 2;
	}
	else
	{
		char	line[8192];
		int		currentLine = 1;
		map_t	lines;

		line[stlsoft_num_elements(line) - 1] = 0;
		line[stlsoft_num_elements(line) - 2] = 0;

		while(fgets(line, stlsoft_num_elements(line), in))
		{
			if(	0 != line[stlsoft_num_elements(line) - 1] ||
				0 != line[stlsoft_num_elements(line) - 2])
			{
				fprintf(stderr, "Line %d too long!\n", currentLine);

				iRet = 3;
				break;
			}

			/* Remove the \n" */
			line[strlen(line) - 1] = 0;

			if(!bShowCount)
			{
				if(lines.find(line) == lines.end())
				{
					fprintf(out, "%s\n", line);
				}
			}

			lines[line]++;
		}

		if(bShowCount)
		{
			for(map_t::const_iterator begin = lines.begin(), end = lines.end(); begin != end; ++begin)
			{
				fprintf(out, "%s\t%d\n", c_str_ptr((*begin).first), (*begin).second);
			}
		}
	}

    return iRet;
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

	fprintf(stderr, "  USAGE 1: %s [-c] [<input>] [<output>]\n", EXE_NAME);
	fprintf(stderr, "    where:\n\n");
	fprintf(stderr, "    -c          -   displays number of occurrences of each unique line\n");
	fprintf(stderr, "    <input>     -   input file, or to stdout if not specified\n");
	fprintf(stderr, "    <output>    -   output file, or to stdout if not specified\n");
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
