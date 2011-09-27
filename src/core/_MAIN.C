/*_ _main.c   */
/* Copyright (C) 1985-1991 by Walter Bright	*/
/* All rights reserved.				*/

#include <tchar.h>
#include <sctchar.h>
#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>

/*********************************
 * This takes
 * argc and argv, and does wild card expansion on any arguments
 * containing a '*' or '?', and then calls main() with a new
 * argc and argv. Any errors result in calling main() with the original
 * arguments intact. Arguments containing wild cards that expand to
 * 0 filenames are deleted. Arguments without wild cards are passed
 * straight through.
 *
 * Arguments which are preceded by a " or ', are passed straight
 * through. (cck)
 */


#ifdef _DLL
#define _argc	(*__p___argc())
#define __argv	(*__p___argv())
#define __wargv	(*__p___wargv())
#else
extern int _argc;
extern _TCHAR **__targv;
#endif
int __wildcard = 1;	/* a dummy that can be used to automatically
			   pull in this module. See macro WILDCARDS
			   in dos.h */

/* Using a name starting with "_STI" causes this function to be put	*/
/* into the static constructor table. (!)				*/

#if _NOCTOR
void _sti_wildcard()
#else
void _STI_wildcard()
#endif
{
	struct FIND  *p;
	int i,nargc, path_size, nargvmax;
	_TCHAR **nargv, path[FILENAME_MAX+1], *cp, *end_path;

	nargc = 0;
	nargvmax = 0;		/* dimension of nargv[]		*/
	nargv = NULL;
	for(i=0; i<_argc; ++i)
	{
	    if (nargc + 2 >= nargvmax)
	    {   nargvmax = nargc + 2;
		nargv = (_TCHAR **) realloc(nargv,nargvmax * sizeof(_TCHAR *));
		if (!nargv)
		    goto noexpand;
	    }

		cp = __targv[i];				/* cck */

		/* if have expandable names */
		if( !(cp[-1]=='"' || cp[-1]=='\'') &&	/* cck */
		     (_tcschr(cp, '*') || _tcschr(cp, '?'))
				)
		{
			end_path = cp + _tcslen(cp);
#ifdef _MBCS
			while (end_path >= cp &&
				*end_path != '\\' &&
				*end_path != '/' &&
				*end_path != ':') {
				end_path = _tcsdec (cp, end_path);
				if (end_path == NULL) {
					end_path = cp - 1;
					break;
				}
			}
#else
			while (	end_path >= cp &&
				*end_path != '\\' &&
				*end_path != '/' &&
				*end_path != ':')
				end_path--;
#endif
			path_size = 0;
			if(end_path >= cp){	/* if got a path */
				path_size = end_path - cp + 1;
				memcpy(path, cp, path_size * sizeof (_TCHAR));
			}
			path[path_size] = 0;
			p = __tfindfirst(cp, 0);
			while (p)
			{
				cp = malloc((path_size + _tcslen(p->name) + 1) * sizeof (_TCHAR));
				if (!cp)
					goto noexpand;
				_tcscpy(cp, path);
				_tcscat(cp, p->name);
				if (nargc + 2 >= nargvmax)
				{   nargvmax = nargc + 2;
				    nargv = (_TCHAR **) realloc(nargv,nargvmax * sizeof(_TCHAR *));
				    if (!nargv)
					goto noexpand;
				}
				nargv[nargc++]  = cp;
				p = __tfindnext();
			}
		}
		else
		    nargv[nargc++] = __targv[i];
	}
	nargv[nargc] = NULL;
	_argc = nargc;
	__targv = nargv;

noexpand:
    ;	/* give up							*/
	/* Partial nargv[] structure is not free'd, but who cares	*/
	/* because we're out of memory anyway				*/
}
