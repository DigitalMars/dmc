// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE:
//    Contains declarations for the toolbar module.
//

//-------------------------------------------------------------------------

// All measurements are in twips. A twip is 1440th of an inch, or 1/20th of a point
#define RM_GETMARGINS       (WM_USER+1)  // lParam points to three int array (L,R,I)
#define RM_SETMARGINS       (WM_USER+2)  // lParam points to three int array (L,R,I)
#define RM_GETTABCOUNT      (WM_USER+3)  // Returns number of tabs in ruler
#define RM_GETTABS          (WM_USER+4)  // wParam sez how many tabs to get, lParam points to int array
#define RM_SETTABS          (WM_USER+5)  // wParam = num tabs, lParam = array of ints

#define IDB_RULER             800 // Bitmap ID to make Ruler Image List
#define ID_RULER              42  // Window ID for ruler

#define RULER_MAXTABS 25
#define SZRULERNAME "cls_framework_ruler"

typedef struct tagNMRULER
  {
  NMHDR nmhdr;
  int   iLeft;
  int   iRight;
  int   iFirstLineIndent;
  }
NMRULER, FAR * LPNMRULER;

#define RN_CHANGEDSETTINGS  4242U

LRESULT MsgRulerCreate(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam);
LRESULT MsgRulerDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
LRESULT MsgRulerEraseBkgnd(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
LRESULT MsgRulerPaint(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
LRESULT MsgRulerLButtonDown(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
LRESULT MsgRulerLButtonUp(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
LRESULT MsgRulerMouseMove(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
LRESULT MsgRulerGetMargins(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
LRESULT MsgRulerSetMargins(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
LRESULT MsgRulerGetTabCount(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
LRESULT MsgRulerGetTabs(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
LRESULT MsgRulerSetTabs(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
