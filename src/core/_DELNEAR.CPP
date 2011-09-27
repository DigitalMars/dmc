/*_ _delete.c   Tue May 30 1989   Modified by: Walter Bright */

#include	<stdlib.h>

/***********************
 * Standard C++ free function.
 * Nothing to it.
 */

#if _WINDOWS
#if __LARGE__

void operator delete(void __near *p)
{
    _nfree(p);
}

#endif
#endif

