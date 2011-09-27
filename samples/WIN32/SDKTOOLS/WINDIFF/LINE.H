
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
 * LINE.H
 */

typedef struct fileline FAR * LINE;

LINE line_new(LPSTR text, int linelength, UINT linenr, LIST list);
void line_delete(LINE line);
void line_reset(LINE line);
BOOL line_compare(LINE line1, LINE line2);
BOOL line_link(LINE line1, LINE line2);
LPSTR line_gettext(LINE line);
int line_gettabbedlength(LINE line, int tabstops);
DWORD line_gethashcode(LINE line);
LINE line_getlink(LINE line);
UINT line_getlinenr(LINE line);
BOOL line_isblank(LINE line);
