//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       fractal.h
//
//  Contents:   global definitions used by MFract
//
//  Classes:
//
//  Functions:  WinMain
//              Exists
//
//  History:    4-28-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __FRACTAL_H__
#define __FRACTAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define IDM_ABOUT           200
#define IDM_HELP            201
#define IDM_LOAD            202
#define IDM_SAVE            203
#define IDM_SAVEBMP         204
#define IDM_EXIT            205
#define IDM_START           302
#define IDM_STOP            303
#define IDM_SELECT_TYPE     304
#define IDM_EDIT_PROPERTIES 305
#define IDM_ZOOM            306
#define IDM_BRING_FORWARD   320
#define IDM_CHANGEDIM       400
#define IDM_LOADPAL         500
#define IDM_SAVEPAL         501
#define IDM_DEFAULTCOLORS   502
#define IDM_CYCLEIN         504
#define IDM_CYCLESTOP       505
#define IDM_CYCLEOUT        506
#define IDM_FRACTAL_TYPE    1000

#define IDC_WIDTH       601
#define IDC_HEIGHT      602
#define IDC_RESIZE      603
#define IDC_RESET       604
#define IDC_QUICKDRAW   605
#define IDC_VGA         606
#define IDC_SVGA        607
#define IDC_1024        608
#define IDC_1280        609
#define IDC_CUSTOM      610

#define MAX_FRACTAL_TYPES   1000

#define IDS_ERROR                2000
#define IDS_OLEINCOMPATIBLE      2001
#define IDS_OLEINITFAILED        2002
#define IDS_TOOMANYTIMERS        2003
#define IDS_GRAPHICSEVENTFAILED  2004
#define IDS_TITLEEVENTFAILED     2005
#define IDS_NOFRACTALENGINE      2006
#define IDS_NOHELPFILE           2007
#define IDS_SAVEBMPFAILED        2008
#define IDS_SAVEBMPSUCCEEDED     2009
#define IDS_LOADFRACTALFAILED    2010
#define IDS_SAVEFRACTALFAILED    2011
#define IDS_SAVEFRACTALSUCCEEDED 2012
#define IDS_REGISTRYERROR        2013

#define COLORTABLESIZE 199

#define HELPFILE_STR    "MFract.hlp"
#define APP_TITLE       "MFract"

#define MAX_STRING_LENGTH   256

int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
int Exists(TCHAR *sz);

#ifdef __cplusplus
}
#endif

#endif // __FRACTAL_H__
