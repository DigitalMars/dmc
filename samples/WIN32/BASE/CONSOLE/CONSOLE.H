
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#define BACKGROUND_WHITE  (WORD) 0x00f0
#define BACKGROUND_CYAN   (WORD) 0x0030
#define FOREGROUND_YELLOW (WORD) 0x0006
#define FOREGROUND_CYAN   (WORD) 0x0003
#define FOREGROUND_WHITE  (WORD) 0x0007

#define PERR(bSuccess, api) {if (!(bSuccess)) perr(__FILE__, __LINE__, \
    api, GetLastError());}

/* function prototypes */
extern int myPuts(HANDLE hConsole, char *s);
extern void showConAPIs(HANDLE hConsole);
extern void putStatusLine(HANDLE hOut, char *buf);
extern void perr(char *szFileName, int line, char *szApiName, DWORD dwError);
extern void cls(HANDLE hConsole);
extern char myGetchar(void);
extern void setConTitle(char *szTitle);
extern SHORT getConX(HANDLE hCon);
extern SHORT getConY(HANDLE hCon);
