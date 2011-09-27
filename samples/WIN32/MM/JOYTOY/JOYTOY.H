/*  joytoy.h - Header file for JOYTOY sample application.
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
 *  Copyright (C) 1993 - 1995 Microsoft Corporation. All Rights Reserved.
 */

/* Function prototypes.
 */
LRESULT FAR PASCAL WndProc(HWND, UINT, WPARAM, LPARAM ) ;
BOOL InitJoyToy(HANDLE hInstance, HANDLE hPrevInstance);
void DrawSight(DWORD lParam);
void DrawHole(HWND hWnd);
