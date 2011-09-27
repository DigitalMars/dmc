/*_ winshell.c	*/

/* A simple way to convert a command line program to a Windows program.
 */

#if _WINDOWS

#include <windows.h>
#include <winshell.h>
#include <stdlib.h>
#include <string.h>

extern int _cdecl main(int argc,char **argv);

//void _cdecl __faterr(char *) {}

extern char * _cdecl _acmdln;
extern char * _cdecl _pgmptr;

#ifdef _DLL
#define _argc	(*__p___argc())
#define _argv	(*__p___argv())
#else
extern int _cdecl _argc;
extern char ** _cdecl _argv;
#endif


int _cdecl _acrtused_winc = 0;	/* prevent WINC from being linked in	*/

static char ERROR_ATOM[] = "EXECUTION_DONE_ERROR";
static char NOERROR_ATOM[] = "EXECUTION_DONE";

int PASCAL WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
HANDLE hInstance;
HANDLE hPrevInstance;
LPSTR lpCmdLine;
int nCmdShow;
{	int exitstatus;

//	_acmdln = lpCmdLine;
//	_setargv();
	exitstatus = main(_argc,_argv);
	win_exit(exitstatus);
	return exitstatus;
}

/*******************************
 * Yield control.
 */

void _cdecl win_yield()
{
	Yield();
}

/*******************************
 * Pass message to parent that we are done.
 */

void win_exit(int exitstatus)
{
    if (exitstatus)
	GlobalAddAtom(ERROR_ATOM);
    else
	GlobalAddAtom(NOERROR_ATOM);
}

/*******************************
 * This is a replacement for the _exec found in the Zortech library.
 * Returns:
 *	-1	error loading program
 *	n	success (n is the exit status of the executed command)
 *		0:	success
 *		1:	failure
 */

int win_spawn(char *command,char *commandline)
{
	struct {
		unsigned  	envSeg;
		LPSTR    	lpcmdLine;
		unsigned far *  lpwShow;
		unsigned far * 	lpwReserved;
        } execParm;
	static WORD awShow[2] = {2, SW_SHOW};
	int childinstance;
	int result;
	char *p;

        ATOM atom;

	execParm.envSeg      = 0;
	execParm.lpcmdLine   = commandline;
	execParm.lpwShow     = (unsigned far *) awShow;
	execParm.lpwReserved = (unsigned far *) NULL;

	if (atom = GlobalFindAtom(ERROR_ATOM))
		GlobalDeleteAtom(atom);
	if (atom = GlobalFindAtom(NOERROR_ATOM))
		GlobalDeleteAtom(atom);

	/* Append .EXE to command because LoadModule is too stupid to	*/
	for (p = command + strlen(command); 1; p--)
	{   if (*p == '.')			/* already has an extension */
	    {
		childinstance = LoadModule(command, &execParm);
		break;
	    }
	    else if (*p == '\\' || *p == ':' || *p == '/' || p <= command)
	    {	/* Append .EXE to command	*/
		static char ext[] = ".EXE";

		p = malloc(strlen(command) + sizeof(ext));
		if (!p)
		    childinstance = 0;		/* out of memory	*/
		else
		{
		    strcpy(p,command);
		    strcat(p,ext);
		    childinstance = LoadModule(p, &execParm);
		    free(p);
		}
		break;
	    }
	}

	if (childinstance >= 32)		/* if load was okay */
	{   while (1)
	    {
		if (atom = GlobalFindAtom(NOERROR_ATOM)) {
			GlobalDeleteAtom(atom);
			result = 0;
			break;
		} else if (atom = GlobalFindAtom(ERROR_ATOM)) {
			GlobalDeleteAtom(atom);
			result = 1;
			break;
		}
		win_yield();
	    }
	}
        else
	    result = -1;

	return result;
}

/*****************************************
 * Dummy out the one in the library.
 */

void _pascal MessageLoop(MSG _far *msg)
{
}

#endif
