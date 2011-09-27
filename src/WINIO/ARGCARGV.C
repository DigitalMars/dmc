/*
ARGCARGV.C -- WinMain->main startup for WINIO library
Changed considerably since the version in MSJ (May 1991)

from "Undocumented Windows"  (Addison-Wesley, 1992)
by Andrew Schulman, Dave Maxey and Matt Pietrek.

Copyright (c) Dave Maxey and Andrew Schulman 1991-1992

MSC/C++ 7.0 has QuickWin library.  It's not adequate for our
purposes, but its main() gets in the way of ours.  Need to
link with ARGCARGV.OBJ; can't put ARGCARGV.OBJ in a library
*/

#include "windows.h"
#include <stdlib.h>
#include <string.h>
#include "winio.h"

#define MAIN_BUFFER 32760

#define argc _argc
#define argv _argv

short _acrtused_winc = 1234;  // Causes linker to pull in automatically for SC++

extern int _argc;
extern char **_argv;
extern int main(int argc, char **argv);

void getexefilename(HANDLE hInst, char *strName);

HANDLE __hInst;
HANDLE __hPrevInst;
LPSTR __lpCmdLine;
int __nCmdShow;
HWND __hMainWnd;
WORD __hAppTimer;
char __szModule[9] = {0};

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance, 
    LPSTR lpCmdLine, int nCmdShow)
    {
    int ret;
    
    __hInst = hInstance;
    __hPrevInst = hPrevInstance;
    __lpCmdLine = lpCmdLine;
    __nCmdShow = nCmdShow;
    
    getexefilename(__hInst, __szModule);
    winio_about(__szModule);    // default; can override
    
    if (! winio_init())
        {
        winio_warn(FALSE, __szModule, "Could not initialize");
        return 1;
        }
    
	 __hMainWnd = winio_window((LPSTR)NULL,MAIN_BUFFER,WW_HASMENU | WW_EXITALLOWED);

    if (__hMainWnd)
        {
        // App timer to allow multitasking
        __hAppTimer = SetTimer(NULL, 0xABCD, 100, NULL);
    
        winio_setcurrent(__hMainWnd);
        
        ret = main(argc, argv);

        winio_end();
    
        if (__hAppTimer)
            KillTimer(NULL, __hAppTimer);
        }
    else
        {
        winio_warn(FALSE, __szModule, "Could not create main window");
        ret = -1;
        }
    
    return ret;
    }


void getexefilename(HANDLE hInst, char *strName)
    {
    char str[128];
    char *p;

    // can use hInst as well as hMod (GetExePtr does the trick)
    GetModuleFileName(hInst, str, 128);
    p = &str[strlen(str) - 1];
    
    for ( ; (p != str) && (*p != '\\'); p--)
        if (*p == '.') *p = 0;
        
    strcpy(strName, *p == '\\' ? ++p : p);
    }
    
