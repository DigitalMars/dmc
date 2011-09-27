
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/


BOOL ExpDir(LPCINFO);
void ExpDirExit(LPCINFO, HANDLE);
BOOL ConstructDirName(LPCINFO, LONG, LPTSTR);
LONG GetDirDepth(LPTSTR, LPTSTR*);
BOOL CollapseDir(LPCINFO, LONG, LONG);
void ConstructLBEntry(LONG, LPTSTR, LPTSTR);
BOOL FullExpand(LPCINFO);
