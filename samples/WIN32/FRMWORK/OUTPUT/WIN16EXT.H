// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE: Defines additions to windows.h included in WIN32, but not
//          in win16.

// Windows NT defines APIENTRY, but 3.x doesn't
#define APIENTRY far pascal

// Windows NT defines the following in windowsx.h
#define GET_WM_COMMAND_ID(w,l) (w)
#define GET_WM_COMMAND_CMD(w,l) HIWORD(l)
#define GET_WM_COMMAND_HWND(w,l) (HWND)(LOWORD(l))
