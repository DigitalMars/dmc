/*	tabsize.c
 *	controls the program wide tab size, which is used by
 *	all routines that need to print tabs themselves.
 *	(_swapl && winc support stuff for windows)
 *
 *	(C) Copyright 1993 Digital Mars
 *	All Rights Reserved
 *	Written by: G. Eric Engstrom
 *
 *	NOTE: recognizes TAB and TABS environment variables
 *	      sets TAB and clears TABS when doing tab_sizeputenv
 */

#include	<tabsize.h>
#include	<string.h>
#include	<stdlib.h>

/* tab_sizeset
 *
 * if newtabsize is within range
 * it sets the tab size to be used by the program
 */
void tab_sizeset(unsigned short newtabsize)
{
    if (newtabsize > 1
      && newtabsize <= 32)
      {
      _tab_size = newtabsize;
      }
}

/* tab_sizeget
 *
 * returns the tab size in use by the system
 */
unsigned short tab_sizeget(void)
{
    return _tab_size;
}

/* tab_sizegetenv
 *
 * returns the tab size specified in the environment or 0 if none was
 *         found or value was out of range
 */
unsigned short tab_sizegetenv(void)
{
char *a;

    if ((a = getenv("TAB")) != NULL		/* TAB has higher precedence than TABS */
      || (a = getenv("TABS")) != NULL)		/* but we accept both */
      {
      int  b;

      b = atoi(a);
      if (b > 1
        && b <= 32)
        {
        return _tab_size = b;
        }
      }
    return 0;
}

/* tab_sizeputenv
 *
 * if newtabsize is within range
 * defines TAB=newtabsize in the environment
 * undefins TABS if it was set
 *
 */
void tab_sizeputenv(unsigned short newtabsize)
{
    if (newtabsize > 1
      && newtabsize <= 32)
      {
      static char tab[]  = "TAB=00";
      static char tabs[] = "TABS=";

      itoa(newtabsize,&tab[4],2);
      if (putenv(tab) == 0)	/* define TAB= user request */
        {
        putenv(tabs);		/* erase TABS if defined */
        }
      }
}
