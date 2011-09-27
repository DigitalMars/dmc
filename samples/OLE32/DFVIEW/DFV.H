//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       DFV.h
//
//  Contents:   globally defined constants, IDs and structures
//
//  History:    6-08-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __DFV_H__
#define __DFV_H__

#include <windows.h>
#include <ole2.h>
#include "message.h"

// String Constants
#define VER_FILEDESCRIPTION_STR     "Windows NT Docfile Viewer"
#define VER_INTERNALNAME_STR        "DocFile Viewer"
#define VER_ORIGINALFILENAME_STR    "DFVIEW.exe"
#define HELPFILE_STR                "OLETools.hlp"
#define MAIN_WINDOW_CLASS_NAME      "MainDFVWindow"
#define MAIN_WINDOW_CLASS_MENU      MainDFVMenu
#define MAIN_WINDOW_CLASS_MENU_STR  "MainDFVMenu"
#define STREAM_VIEW_CLASS_NAME      "Stream Viewer"

// Menu Command Identifiers
#define IDM_EXIT                    1000
#define IDM_OPEN                    1001
#define IDM_CLOSE                   1002
#define IDM_EXPAND                  1100
#define IDM_EXPANDBRANCH            1101
#define IDM_EXPANDALL               1102
#define IDM_COLLAPSE                1103
#define IDM_COLLAPSEALL             1104
#define IDM_TOGGLE                  1105
#define IDM_HELP                    1200
#define IDM_ABOUT                   1201

// Listbox Identifiers
#define IDC_LISTBOX                 4000
#define IDC_STREAMVIEW              4001

// Bitmap Identifiers
#define BMP_STREAM                  3000
#define BMP_STORAGE                 3001
// String Identifiers
#define IDS_ERROR                   2000
#define IDS_NOHELPFILE              2001
#define IDS_OLEINCOMPATIBLE         2002
#define IDS_OLEINITFAILED           2003
#define IDS_ENUMSTATSTGFAILED       2004
#define IDS_OPENSTORAGEFAILED       2005
#define IDS_OPENSTREAMFAILED        2006
#define IDS_ENUMELEMENTSFAILED      2007
#define IDS_INSERTSTRINGFAILED      2008
#define IDS_LOADBITMAPSFAILED       2009
#define IDS_STGOPENSTORAGEFAILED    2010
#define IDS_STMSTATFAILED           2011
#define IDS_STMSEEKFAILED           2012
#define IDS_STMREADFAILED           2013
#define IDS_STMTITLETEXT            2014
#define IDS_OUTOFMEMORY             2015

#endif // __DFV_H__
