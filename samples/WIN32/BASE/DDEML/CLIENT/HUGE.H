
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/


// header file for HUGE data handle testing module


// PROCS

HDDEDATA CreateHugeDataHandle(LONG length, LONG seed, LONG mult, LONG add,
        HSZ hszItem, DWORD wFmt, DWORD afCmd);
BOOL CheckHugeData(HDDEDATA hData);
