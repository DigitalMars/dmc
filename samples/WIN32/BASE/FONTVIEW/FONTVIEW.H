
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#define IDM_SYSMENU  666

#define IDM_FILE        100
#define IDM_NEW             IDM_FILE+1
#define IDM_OPEN            IDM_FILE+2
#define IDM_CLOSE           IDM_FILE+3
#define IDM_SAVE            IDM_FILE+4
#define IDM_SAVEAS          IDM_FILE+5
#define IDM_PRINT           IDM_FILE+6
#define IDM_PRINTSET        IDM_FILE+7
#define IDM_EXIT            IDM_FILE+8

#define IDM_EDIT        110
#define IDM_UNDO            IDM_EDIT+1
#define IDM_CUT             IDM_EDIT+2
#define IDM_COPY            IDM_EDIT+3
#define IDM_PASTE           IDM_EDIT+4
#define IDM_DELETE          IDM_EDIT+5
#define IDM_FIND            IDM_EDIT+6
#define IDM_REPLACE         IDM_EDIT+7

#define IDM_VIEW        120
#define IDM_CHARSET         IDM_VIEW+1
#define IDM_GLYPH           IDM_VIEW+2
#define IDM_TOOLBAR         IDM_VIEW+3
#define IDM_STATUSBAR       IDM_VIEW+4

#define IDM_DIALOG        130
#define IDM_CREATEFONT      IDM_DIALOG+1
#define IDM_TEXTMETRIC      IDM_DIALOG+2
#define IDM_ENUMFONTS       IDM_DIALOG+3
#define IDM_OUTLINEMETRICS  IDM_DIALOG+4

#define IDM_HELP        140
#define IDM_HELP_CONTENTS   IDM_HELP+1
#define IDM_HELP_INDEX      IDM_HELP+2
#define IDM_HELP_OVERVIEW   IDM_HELP+3
#define IDM_HELP_GLOSSARY   IDM_HELP+4
#define IDM_HELP_TUTORIAL   IDM_HELP+5
#define IDM_HELP_DEMO       IDM_HELP+6
#define IDM_HELP_HELP       IDM_HELP+7
#define IDM_ABOUT           IDM_HELP+8

#define ID_TOOLBAR      150
#define ID_CREATEPARAM      ID_TOOLBAR+1
#define ID_CREATEVALUE      ID_TOOLBAR+2
#define ID_ZOOM                         ID_TOOLBAR+3

#define ID_STATUSBAR    160
#define ID_MENUFIELD        ID_STATUSBAR+1
#define ID_TIMEFIELD        ID_STATUSBAR+2
#define ID_NUMFIELD         ID_STATUSBAR+3
#define ID_OVRFIELD         ID_STATUSBAR+4
#define ID_SCROLLFIELD      ID_STATUSBAR+5
#define ID_CAPSFIELD        ID_STATUSBAR+6

#define CFP_BASE        200
#define CFP_HEIGHT          CFP_BASE+0
#define CFP_WIDTH           CFP_BASE+1
#define CFP_ESCAPEMENT      CFP_BASE+2
#define CFP_ORIENTATION     CFP_BASE+3
#define CFP_WEIGHT          CFP_BASE+4
#define CFP_ITALIC          CFP_BASE+5
#define CFP_UNDERLINE       CFP_BASE+6
#define CFP_STRIKEOUT       CFP_BASE+7
#define CFP_CHARSET         CFP_BASE+8
#define CFP_OUTPUTPRECISION CFP_BASE+9
#define CFP_CLIPPRECISION   CFP_BASE+10
#define CFP_QUALITY         CFP_BASE+11
#define CFP_PITCH           CFP_BASE+12
#define CFP_FAMILY          CFP_BASE+13
#define CFP_FACENAME        CFP_BASE+14

#define CFD_BASE        220
#define CFD_HEIGHT          CFD_BASE+0
#define CFD_WIDTH           CFD_BASE+1
#define CFD_ESCAPEMENT      CFD_BASE+2
#define CFD_ORIENTATION     CFD_BASE+3
#define CFD_WEIGHT          CFD_BASE+4
#define CFD_ITALIC          CFD_BASE+5
#define CFD_UNDERLINE       CFD_BASE+6
#define CFD_STRIKEOUT       CFD_BASE+7
#define CFD_CHARSET         CFD_BASE+8
#define CFD_OUTPUTPRECISION CFD_BASE+9
#define CFD_CLIPPRECISION   CFD_BASE+10
#define CFD_QUALITY         CFD_BASE+11
#define CFD_PITCHANDFAMILY  CFD_BASE+12
#define CFD_FACENAME        CFD_BASE+13

#define CFD_DEFAULT         CFD_BASE+14

#define TMD_BASE        240
#define TMD_HEIGHT          TMD_BASE+0
#define TMD_ASCENT          TMD_BASE+1
#define TMD_DESCENT         TMD_BASE+2
#define TMD_INTERNAL        TMD_BASE+3
#define TMD_EXTERNAL        TMD_BASE+4
#define TMD_AVEWIDTH        TMD_BASE+5
#define TMD_MAXWIDTH        TMD_BASE+6
#define TMD_WEIGHT          TMD_BASE+7
#define TMD_ITALIC          TMD_BASE+8
#define TMD_UNDERLINE       TMD_BASE+9
#define TMD_STRUCKOUT       TMD_BASE+10
#define TMD_FIRSTCHAR       TMD_BASE+11
#define TMD_LASTCHAR        TMD_BASE+12
#define TMD_DEFAULTCHAR     TMD_BASE+13
#define TMD_BREAKCHAR       TMD_BASE+14
#define TMD_PITCHANDFAMILY  TMD_BASE+15
#define TMD_CHARSET         TMD_BASE+16
#define TMD_OVERHANG        TMD_BASE+17
#define TMD_DIGITIZEDASPECTX TMD_BASE+18
#define TMD_DIGITIZEDASPECTY TMD_BASE+19

#define TMD_SET             TMD_BASE+20


#define ED_BASE         260
#define ED_PREV             ED_BASE+0
#define ED_NEXT             ED_BASE+1
#define ED_TYPE             ED_BASE+2
#define ED_SET              ED_BASE+3
#define ED_HEX              ED_BASE+4
#define ED_SAMPLE           ED_BASE+5
