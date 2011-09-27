/* ANres.h: resource defines

   Copyright 1993-1995 Microsoft Corporation.  All rights reserved.
   Microsoft Confidential.
*/

#ifndef _INCLUDE_ANRESH
#define _INCLUDE_ANRESH

#include <windows.h>

/******************* String Definitions. ***************************/

#define RS_FILTER                   10
#define RS_OFNREADTITLE             11
#define RS_OFNWRITETITLE            12

/******************* Menu Definitions. *****************************/
#define IDM_EXIT                    1000
#define IDM_OUTPUT                  1001
#define IDM_OPEN                    1002
#define IDM_SAVE                    1003
#define IDM_CBDISP                  1004
#define IDM_DLG                     1100
#define IDM_DRAWEX                  1200
#define IDM_DRAW                    1201
#define IDM_DRAWPARTIAL             1202

/******************* Icons/Cursors/Bitmaps ***************************/

/******************* Dialog Definitions. *************************/
#define IDD_ANIMATE                 100
#define IDD_STSTRK0                 200
#define IDD_ETSTRK0                 201
#define IDD_STPT0                   202
#define IDD_ETPT0                   203
#define IDD_STSTRK1                 204
#define IDD_ETSTRK1                 205
#define IDD_STPT1                   206
#define IDD_ETPT1                   207
#define IDD_CHSKIPUP                208
#define IDD_CHAUTOREPEAT            209
#define IDD_GBCB                    210
#define IDD_RBCBNEVER               211
#define IDD_RBCB250MS               212
#define IDD_RBCB1SEC                213
#define IDD_RBCBSTROKE              214
#define IDD_STSPEED                 220
#define IDD_ETSPEED                 221
#define IDD_STPCT                   222
#define IDD_SBSPEED                 223
#define IDD_GBTERM                  230
#define IDD_RBTERMTIMEOUT           231
#define IDD_RBTERMRECT              232
#define IDD_GBRENDERING             240
#define IDD_RBRENDERSCALE           241
#define IDD_RBRENDERCLIP            242
#define IDD_PBCLEAR                 250
#define IDD_PBEXIT                  251
#define IDD_PBREDRAW                252
#endif   // #define _INCLUDE_ANRESH
