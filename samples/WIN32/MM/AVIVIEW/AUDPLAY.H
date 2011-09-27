/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/


BOOL aviaudioPlay(HWND hwnd, PAVISTREAM pavi, LONG lStart, LONG lEnd, BOOL fWait);
void aviaudioMessage(HWND hwnd, unsigned msg, WPARAM wParam, LONG lParam);
void aviaudioStop(void);
LONG aviaudioTime(void);
