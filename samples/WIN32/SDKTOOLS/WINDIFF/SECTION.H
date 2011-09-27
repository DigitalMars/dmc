
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
 * SECTION.H
 */

/* handle to a section */
typedef struct section FAR * SECTION;


SECTION section_new(LINE first, LINE last, LIST list);
void section_delete(SECTION section);
BOOL section_match(SECTION section1, SECTION section2);
LINE section_getfirstline(SECTION section);
LINE section_getlastline(SECTION section);
SECTION section_getlink(SECTION section);
SECTION section_getcorrespond(SECTION section);
void section_setstate(SECTION section, int state);
int section_getstate(SECTION section);
int section_getlinecount(SECTION section);
int section_getleftbasenr(SECTION section);
int section_getrightbasenr(SECTION section);
LIST section_makelist(LIST linelist, BOOL left);
void section_deletelist(LIST sections);
LIST section_makecomposite(LIST secsleft, LIST secsright);
BOOL section_matchlists(LIST secsleft, LIST secsright);



