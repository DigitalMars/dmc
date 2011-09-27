
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*
 * FILE.H
 */

/* handle to filedata */
typedef struct filedata FAR * FILEDATA;

FILEDATA file_new(DIRITEM fiName, BOOL bRead);
DIRITEM file_getdiritem(FILEDATA fi);
void file_delete(FILEDATA fi);
LIST file_getlinelist(FILEDATA fi);
void file_discardlines(FILEDATA fi);
void file_reset(FILEDATA fi);

