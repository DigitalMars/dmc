
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* definitions */

#define DRIVE_REMOVEABLE 2
#define DRIVE_FIXED      3
#define DRIVE_REMOTE     4
#define DRIVE_CDROM      5
#define DRIVE_RAMDISK    6

/* procedure definitions */

BOOL   CheckRM      (LPTSTR);
VOID   GVI          (LPTSTR, LPTSTR, DWORD);
LPTSTR UpdateBuffer (LPTSTR);
VOID   main         (VOID);


