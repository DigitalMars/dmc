/*_ getenv.c   Modified by: Walter Bright
 * Copyright (C) 1991-1995 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include	<stdlib.h>
#include	<string.h>
#include	<dos.h>

#if __OS2__
#include	"os2lib.h"
#endif

#ifdef _WIN32
#include <windows.h>
#include "semlock.h"
#else

#if __I86__ >= 3
extern char *_aenvptr;
#else
extern unsigned short aenvseg;
char *_envbuf;			/* pointer to malloced buffer		*/
#endif

char *_envptr;			/* pointer to start of environment string */
#endif

#include	"mt.h"


/********************************
 * Get environment string
 * Return pointer to it.
 */
 
#ifdef _WIN32

static void _storeosenvironstring(char *var,char *value)
{
    static char **osvaluetable, **osvartable;
    static unsigned osenviron_count;
    unsigned i;
    
    
    if (osvartable)
    {
        for (i = 0; i < osenviron_count; i++)
        {
            if (_stricmp(osvartable[i],var) == 0)
            {
                break;
            }
        }
        
        /* found it */
        if (i < osenviron_count)
        {
            if (strcmp(osvaluetable[i],value))
            {
                free(osvaluetable[i]);
                osvaluetable[i] = value;
            }
            return;
        }
    }
    
    
    osenviron_count++;
    osvaluetable = realloc(osvaluetable,osenviron_count * sizeof(char *));
    osvartable = realloc(osvartable,osenviron_count * sizeof(char *));
    
    if (osvartable && osvaluetable)
    {
        osvaluetable[osenviron_count - 1] = value;
        osvartable[osenviron_count - 1] = var;
    }
}


static char * _getosenv(const char * var)
{
    char *retval = NULL;
    
    if (var)
    {
        unsigned long allocSize = GetEnvironmentVariable(var,NULL,0);
        if (allocSize)
        {
            retval = malloc(allocSize+1);
            if (retval)
            {
                if(!GetEnvironmentVariable(var,retval,allocSize))
                {
                    free(retval);
                    retval = NULL;
                }
                else
                {
                    _storeosenvironstring(strdup(var),retval);
                }
            }
        }
    }
    return retval;
}

#endif


char *getenv(const char *var)
{
#ifdef _WIN32
    char **envptr;
    char *temp;
    char *ptr = NULL;
    
    LockSemaphore(_semEnvironment);
    
    if (!_environ || !var)
      goto Lret;
    
    for (envptr = _environ; *envptr; envptr++)
    {
        temp = strchr(*envptr,'=');
        if (!temp)
          continue;
          
        if ((temp - (*envptr)) == strlen(var))
        {
            if (strncmpi((*envptr),var,strlen(var)) == 0)
            {
                ptr = (temp + 1);
                goto Lret;
            }
        }
    }
    
    // if not found, try os...
    ptr = _getosenv(var);
    
Lret:
    UnlockSemaphore(_semEnvironment);
    return ptr;

#else
    char *p;
    size_t i;

    LockSemaphore(_semEnvironment)

#if __INTSIZE == 4
    if (_envptr == 0)
	_envptr = _aenvptr;		/* initialize			*/
#else
    if (FP_SEG(_envptr) == 0)
	FP_SEG(_envptr) = aenvseg;	/* initialize			*/
#endif
    p = _envptr;
    i = strlen(var);

    while (1)
    {
	if (strncmp(var,p,i) == 0 && p[i] == '=')
	{   /* Found a match	*/
	    p += i + 1;
	    break;
	}
	p = strchr(p,0) + 1;		/* scan past terminating 0	*/
	if (!*p)			/* end of environment string	*/
	{   p = NULL;
	    break;
	}
    }
    UnlockSemaphore(_semEnvironment)
    return p;
#endif
}

/***********************
 * Initialize _envptr
 */

#ifndef _WIN32

#if _NOCTOR
void _sti_getenv()
#else
void _STI_getenv()
#endif
{
    getenv("A");
}


void _STD_getenv()
{
}

#endif
