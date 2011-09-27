/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
 * 
 **************************************************************************/

#define ID_BAR	     100
#define ID_STATUS1   10
#define ID_STATUS2   11
#define ID_STATUS3   12
#define ID_STATUS4   13

#ifndef RC_INVOKED
	BOOL CALLBACK ProInit(HANDLE,HANDLE);
	HWND CALLBACK ProOpen(HWND);
	BOOL CALLBACK ProClose(void);
	BOOL CALLBACK ProSetText (int,LPSTR);
	BOOL CALLBACK ProSetBarPos(UINT);
#endif
    
