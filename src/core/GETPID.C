/*_ getpid.c */

#include	<process.h>

#if __OS2__
#include	<os2lib.h>
#elif __NT__
#include	<windows.h>
#else
#include	<dos.h>
#include <stdlib.h>
#endif

int getpid()
{
#if __NT__
    return GetCurrentProcessId ();
#elif __OS2__ && __INTSIZE == 4
    unsigned **ptib;
    unsigned *ppib;

    DosGetInfoBlocks(&ptib,&ppib);
    return *ppib;
#elif __OS2__
    unsigned buffer[3];

    DosGetPID(buffer);
    return buffer[0];		/* PID of current process	*/
#else
    return _psp;
#endif
}
